module Element = Webapi.Dom.Element;

module Css = {
  let intToPx = (n) => string_of_int(n) ++ "px";
  let floatToPx = (n) => string_of_float(n) ++ "px";
};

type scroll = {
  width: int,
  height: int,
  top: int,
  left: int
};

let nodeToRect = (node) =>
  Element.ofNode(node) |> Js.Option.getExn |> Element.getBoundingClientRect;

let elemScroll = (element) => {
  width: Element.scrollWidth(element),
  height: Element.scrollHeight(element),
  top: Element.scrollTop(element),
  left: Element.scrollLeft(element),
};