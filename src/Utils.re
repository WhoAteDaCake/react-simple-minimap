module Element = Webapi.Dom.Element;

module Css = {
  let intToPx = n => string_of_int(n) ++ "px";
  let floatToPx = n => string_of_float(n) ++ "px";
};

type scroll = {
  width: int,
  height: int,
  top: int,
  left: int
};

let nodeToRect = node =>
  Element.ofNode(node) |> Js.Option.getExn |> Element.getBoundingClientRect;

let elemScroll = element => {
  width: Element.scrollWidth(element),
  height: Element.scrollHeight(element),
  top: Element.scrollTop(element),
  left: Element.scrollLeft(element)
};

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

Js.log(Js.Date.now());

let throttle = (f, wait) => {
  let timeout = ref(None);
  (x, y) =>
    switch timeout^ {
    | Some(time) =>
      if (time -. Js.Date.now() >= wait) {
        f(x, y);
        timeout := Some(Js.Date.now());
      }
    | None =>
      f(x, y);
      timeout := Some(Js.Date.now());
    };
};