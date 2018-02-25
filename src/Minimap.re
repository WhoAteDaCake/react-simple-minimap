type state = {mapRef: ref(option(Dom.element))};

type action =
  | GetRef;

let component = ReasonReact.reducerComponent("ReasonReactExample");

let setSectionRef = (theRef, {ReasonReact.state}) => state.mapRef := Js.Nullable.to_opt(theRef);

let make = (~age, _children) => {
  ...component,
  initialState: () => {mapRef: ref(None)},
  reducer: (action, state) =>
    switch action {
    | GetRef => ReasonReact.Update({...state, mapRef: state.mapRef})
    },
  render: (self) => {
    Js.log(self.state);
    <div ref=(self.handle(setSectionRef))> (ReasonReact.stringToElement(age)) </div>
  }
};

let default =
  ReasonReact.wrapReasonForJs(~component, (_jsProps) => make(~age=_jsProps##age, [||]));