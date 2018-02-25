open Config;

module Element = Webapi.Dom.Element;

type state = {
  mapRef: ref(option(Dom.element)),
  sourceRef: ref(option(Dom.element)),
  containerRef: ref(option(Dom.element)),
  init: bool
};

type action =
  | GetRef;

let component = ReasonReact.reducerComponent("ReasonReactExample");

let setMapRef = (theRef, {ReasonReact.state}) => state.mapRef := Js.Nullable.to_opt(theRef);

let setSourceRef = (theRef, {ReasonReact.state}) => state.sourceRef := Js.Nullable.to_opt(theRef);

let setContainerRef = (theRef, {ReasonReact.state}) =>
  state.containerRef := Js.Nullable.to_opt(theRef);

let text = ReasonReact.stringToElement("minimap");

let initialState = () => {
  mapRef: ref(None),
  sourceRef: ref(None),
  containerRef: ref(None),
  init: false
};

let make = (~width, ~height, ~selector="mark", _children) => {
  let init = (state) => {
    let sourceDimensions =
      switch state.sourceRef^ {
      | Some(r) => Some(dimensions(r, selector, width, height))
      | None => None
      };
    /* Js.log(sourceDimensions); */
    ()
    /* let {scrollWidth, scrollHeight, scrollTop, scrollLeft} =  */
  };
  let style = ReactDOMRe.Style.make(~width=intToPx(width), ~height=intToPx(height), ());
  {
    ...component,
    initialState,
    reducer: ((), state) => ReasonReact.Update({...state, init: true}),
    didMount: ({state}) => {
      let newState = init(state);
      ReasonReact.NoUpdate
    },
    render: (self) =>
      <div className="minimap-root" ref=(self.handle(setSourceRef))>
        <div className="minimap" style ref=(self.handle(setMapRef))> text </div>
        <div className="content-container" ref=(self.handle(setContainerRef))> _children </div>
      </div>
  }
};

let default =
  ReasonReact.wrapReasonForJs(
    ~component,
    (_jsProps) => make(~width=_jsProps##width, ~height=_jsProps##height, _jsProps##children)
  );