module Element = Webapi.Dom.Element;

module DomRect = Webapi.Dom.DomRect;

module NodeList = Webapi.Dom.NodeList;

type scroll = {
  width: int,
  height: int,
  top: int,
  left: int
};

type dimension = {
  scroll,
  rectange: Dom.domRect,
  nodes: Dom.nodeList
};

let dimensions = (element, selector, width, height) : dimension => {
  let scroll = {
    width: Element.scrollWidth(element),
    height: Element.scrollWidth(element),
    top: Element.scrollTop(element),
    left: Element.scrollLeft(element)
  };
  let rectange = Element.getBoundingClientRect(element);
  let nodes = Element.querySelectorAll(selector, element);
  /*  */
  let ratioX = width / scroll.width;
  let ratioY = height / scroll.height;
  let nodeList =
    Array.map(
      (node) => {
        let node = Element.ofNode(node) |> Js.Option.getExn |> Element.getBoundingClientRect;
        let (width, height, left, top) = (
          DomRect.width(node),
          DomRect.height(node),
          DomRect.left(node),
          DomRect.top(node)
        );
        /* let {width, height, left, top} = node; */
        Js.log((width, height, left, top));
        ()
      },
      NodeList.toArray(nodes)
    );
  {scroll, rectange, nodes}
};

let intToPx = (num) => string_of_int(num) ++ "px";