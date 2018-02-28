module Element = Webapi.Dom.Element;

module DomRect = Webapi.Dom.DomRect;

type state = {
  mapRef: ref(option(Dom.element)),
  sourceRef: ref(option(Dom.element)),
  containerRef: ref(option(Dom.element)),
  minimapStyle: ReactDOMRe.style,
  markers: Markers.markers,
  viewPort: ReasonReact.reactElement
};

type action =
  | GetRef
  | SetViewPort(ReasonReact.reactElement);

let component = ReasonReact.reducerComponent("ReasonReactExample");

let initialState = () => {
  mapRef: ref(None),
  sourceRef: ref(None),
  containerRef: ref(None),
  markers: [||],
  minimapStyle: ReactDOMRe.Style.make(),
  viewPort: ReasonReact.nullElement
};

let setMapRef = (theRef, {ReasonReact.state}) =>
  state.mapRef := Js.Nullable.to_opt(theRef);

let setSourceRef = (theRef, {ReasonReact.state}) =>
  state.sourceRef := Js.Nullable.to_opt(theRef);

let setContainerRef = (theRef, {ReasonReact.state}) =>
  state.containerRef := Js.Nullable.to_opt(theRef);

let resyncReducer =
  Utils.throttle4(
    (minimapWidth, minimapHeight, _, self) =>
      switch self.ReasonReact.state.sourceRef^ {
      | Some(re) =>
        let style = Handlers.resync(re, minimapWidth, minimapHeight);
        let viewPort = <div style className="minimap-viewport" />;
        self.ReasonReact.send(SetViewPort(viewPort));
      | None => Js.log("Could not find source ref")
      },
    50
  );

let calMinimapStyle = (element, width, height) => {
  let (rect, scroll) = Utils.elemRectAndScroll(element);
  let y = DomRect.top(rect);
  let x = DomRect.left(rect) + scroll.width - width - 5;
  Utils.Css.coordsStyle(IntVals(width, height, y, x));
};

/* Calculate minimap props inside this file so it better represents the structure?s */
/* Separate marking creations vs minimap position calculation */
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
    reducer: (action, state) =>
      switch action {
      | SetViewPort(viewPort) => ReasonReact.Update({...state, viewPort})
      | _ => ReasonReact.NoUpdate
      },
    didMount: ({state}) => {
      let markersO =
        switch state.sourceRef^ {
        | Some(rf) =>
          Some(Markers.create(rf, selector, width, height, markerColor))
        | _ => None
        };
      let viewPortO =
        switch state.sourceRef^ {
        | Some(rf) =>
          let style = Handlers.resync(rf, width, height);
          Some(<div style className="minimap-viewport" />);
        | None => None
        };
      let minimapStyleO =
        switch state.sourceRef^ {
        | Some(rf) => Some(calMinimapStyle(rf, width, height))
        | None => None
        };
      switch (markersO, viewPortO, minimapStyleO) {
      | (Some(markers), Some(viewPort), Some(minimapStyle)) =>
        ReasonReact.Update({...state, markers, viewPort, minimapStyle})
      | _ => ReasonReact.NoUpdate
      };
    },
    render: ({state, handle}) =>
      <div
        className="minimap-root"
        ref=(handle(setSourceRef))
        onScroll=(handle(resyncReducer(width, height)))>
        <div
          className="minimap" style=state.minimapStyle ref=(handle(setMapRef))>
          state.viewPort
          <Fragment> ...state.markers </Fragment>
        </div>
        <div className="content-container" ref=(handle(setContainerRef))>
          _children
        </div>
      </div>
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