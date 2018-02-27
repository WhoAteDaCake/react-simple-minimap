/* open Config; */
module Element = Webapi.Dom.Element;

module DomRect = Webapi.Dom.DomRect;

type state = {
  mapRef: ref(option(Dom.element)),
  sourceRef: ref(option(Dom.element)),
  containerRef: ref(option(Dom.element)),
  minimapProps: Markers.minimapProps,
  markers: Markers.markers,
  viewPort: ReasonReact.reactElement
};

type action =
  | GetRef
  | SetViewPort(ReasonReact.reactElement);

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
  },
  viewPort: ReasonReact.nullElement
};

let roundedifToF = (a, b) => Maths.Mult.ifToF(a, b) |> Maths.frnd;

let resync = (source, width, height) => {
  let rect = Element.getBoundingClientRect(source);
  let scroll = Utils.elemScroll(source);
  let scaleX = Maths.Div.iiToF(width, scroll.width);
  let scaleY = Maths.Div.iiToF(height, scroll.height);
  let lW = roundedifToF(DomRect.width(rect), scaleX);
  let lH = roundedifToF(DomRect.height(rect), scaleY);
  let viewX = roundedifToF(scroll.left, scaleX) |> Utils.Css.floatToPx;
  let viewY = roundedifToF(scroll.top, scaleY) |> Utils.Css.floatToPx;
  let viewWidth = min(float_of_int(width), lW) |> Utils.Css.floatToPx;
  let viewHeight = min(float_of_int(height), lH) |> Utils.Css.floatToPx;
  (viewX, viewY, viewWidth, viewHeight);
};

/* TODO Group marker data to tuple */
let init = (state, width, height, selector, markerColor) =>
  switch state.sourceRef^ {
  | Some(r) => Some(Markers.create(r, selector, width, height, markerColor))
  | _ => None
  };

/* Calculate minimap props inside this file so it better represents the structure?s */
let make =
    (~width, ~height, ~selector="mark", ~markerColor="yellow", _children) => {
  if (Js.typeof(width) != "number") {
    Js.Exn.raiseError("Minimap expected width to be a number");
  } else if (Js.typeof(height) != "number") {
    Js.Exn.raiseError("Minimap expected height to be a number");
  };
  let resyncReducer =
    Utils.throttle(
      (event, self) =>
        switch self.ReasonReact.state.sourceRef^ {
        | Some(re) =>
          let (left, top, width, height) = resync(re, width, height);
          let style = ReactDOMRe.Style.make(~width, ~height, ~left, ~top, ());
          let viewPort = <div style className="minimap-viewport" />;
          self.ReasonReact.send(SetViewPort(viewPort));
        | None => Js.log("Could not find source ref")
        },
      150
    );
  {
    ...component,
    initialState,
    reducer: (action, state) =>
      switch action {
      | SetViewPort(viewPort) => ReasonReact.Update({...state, viewPort})
      | _ => ReasonReact.NoUpdate
      },
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
          self.state.viewPort
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