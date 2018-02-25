module Element = Webapi.Dom.Element;

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
  /* let nodeList = Array.map(() => 1, Webapi.Dom.NodeList.toArray(nodes));
     /* Js.log(nodeList); */ */
  /* Js.log(21); */
  {scroll, rectange, nodes}
};

let intToPx = (num) => string_of_int(num) ++ "px";