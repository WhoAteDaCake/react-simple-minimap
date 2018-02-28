module Element = Webapi.Dom.Element;

module Css = {
  let intToPx = n => string_of_int(n) ++ "px";
  let floatToPx = n => string_of_float(n) ++ "px";
  type styleInput =
    | IntVals(int, int, int, int)
    | FloatVals(float, float, float, float);
  let coordsStyle = (input: styleInput) =>
    switch input {
    | IntVals(width, height, top, left) =>
      ReactDOMRe.Style.make(
        ~width=intToPx(width),
        ~height=intToPx(height),
        ~top=intToPx(top),
        ~left=intToPx(left),
        ()
      )
    | FloatVals(width, height, top, left) =>
      ReactDOMRe.Style.make(
        ~width=floatToPx(width),
        ~height=floatToPx(height),
        ~top=floatToPx(top),
        ~left=floatToPx(left),
        ()
      )
    };
};

type scroll = {
  width: int,
  height: int,
  top: int,
  left: int
};

let nodeToElem = node => Element.ofNode(node) |> Js.Option.getExn;

let nodeToRect = node => nodeToElem(node) |> Element.getBoundingClientRect;

let elemScroll = element => {
  width: Element.scrollWidth(element),
  height: Element.scrollHeight(element),
  top: Element.scrollTop(element),
  left: Element.scrollLeft(element)
};

let elemRectAndScroll = element => (
  Element.getBoundingClientRect(element),
  elemScroll(element)
);

let debounce = (f, wait) => {
  let timeout = ref(None);
  (x, y) => {
    switch timeout^ {
    | Some(id) =>
      Js.Global.clearTimeout(id);
      timeout := None;
    | None => ()
    };
    timeout :=
      Some(
        Js.Global.setTimeout(
          () => {
            timeout := None;
            f(x, y);
          },
          wait
        )
      );
  };
};

let throttle = (f, t) => {
  let timeout = ref(None);
  let wait = float_of_int(t);
  (x, y) => {
    Js.log(timeout^);
    switch timeout^ {
    | Some(time) =>
      if (Js.Date.now() -. time >= wait) {
        f(x, y);
        timeout := Some(Js.Date.now());
      } else {
        ();
      }
    | None =>
      f(x, y);
      timeout := Some(Js.Date.now());
    };
  };
};

let throttle4 = (f, t) => {
  let timeout = ref(None);
  let wait = float_of_int(t);
  (x, y, z, c) =>
    switch timeout^ {
    | Some(time) =>
      if (Js.Date.now() -. time >= wait) {
        f(x, y, z, c);
        timeout := Some(Js.Date.now());
      } else {
        ();
      }
    | None =>
      f(x, y, z, c);
      timeout := Some(Js.Date.now());
    };
};