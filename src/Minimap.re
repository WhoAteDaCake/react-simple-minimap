/* open Config; */
module Element = Webapi.Dom.Element;

type state = {
  mapRef: ref(option(Dom.element)),
  sourceRef: ref(option(Dom.element)),
  containerRef: ref(option(Dom.element)),
  minimapProps: Markers.minimapProps,
  markers: Markers.markers
};

type action =
  | GetRef;

let component = ReasonReact.reducerComponent("ReasonReactExample");

let setMapRef = (theRef, {ReasonReact.state}) =>
  state.mapRef := Js.Nullable.to_opt(theRef);

let setSourceRef = (theRef, {ReasonReact.state}) =>
  state.sourceRef := Js.Nullable.to_opt(theRef);

let setContainerRef = (theRef, {ReasonReact.state}) =>
  state.containerRef := Js.Nullable.to_opt(theRef);

let text = ReasonReact.stringToElement("minimap");

let initialState = () => {
  mapRef: ref(None),
  sourceRef: ref(None),
  containerRef: ref(None),
  markers: [||],
  minimapProps: {
    top: 1,
    left: 1
  }
};

/* TODO Group marker data to tuple */
let init = (state, width, height, selector, markerColor) =>
  switch state.sourceRef^ {
  | Some(r) => Some(Markers.create(r, selector, width, height, markerColor))
  | _ => None
  };

let resync = (a, b) => Js.log((a, b));

let resyncI = Utils.throttle((a, b) => resync(a, b), 2000);

let resyncReducer = (e, {ReasonReact.state}) => resyncI(e, state);

/* Calculate minimap props inside this file so it better represents the structure?s */
let make =
    (~width, ~height, ~selector="mark", ~markerColor="yellow", _children) => {
  if (Js.typeof(width) != "number") {
    Js.Exn.raiseError("Minimap expected width to be a number");
  } else if (Js.typeof(height) != "number") {
    Js.Exn.raiseError("Minimap expected height to be a number");
  };
  {
    ...component,
    initialState,
    reducer: ((), state) => ReasonReact.NoUpdate,
    didMount: ({state}) =>
      switch (init(state, width, height, selector, markerColor)) {
      | Some({markers, minimapProps}) =>
        ReasonReact.Update({...state, markers, minimapProps})
      | None => ReasonReact.NoUpdate
      },
    render: self => {
      let style =
        ReactDOMRe.Style.make(
          ~width=Utils.Css.intToPx(width),
          ~height=Utils.Css.intToPx(height),
          ~top=Utils.Css.intToPx(self.state.minimapProps.top),
          ~left=Utils.Css.intToPx(self.state.minimapProps.left),
          ()
        );
      <div
        className="minimap-root"
        ref=(self.handle(setSourceRef))
        onScroll=(self.handle(resyncReducer))>
        <div className="minimap" style ref=(self.handle(setMapRef))>
          <Fragment> ...self.state.markers </Fragment>
        </div>
        <div className="content-container" ref=(self.handle(setContainerRef))>
          _children
        </div>
      </div>;
    }
  };
};

let default =
  ReasonReact.wrapReasonForJs(~component, _jsProps =>
    make(
      ~width=_jsProps##width,
      ~height=_jsProps##height,
      ~markerColor=_jsProps##markerColor,
      _jsProps##children
    )
  );