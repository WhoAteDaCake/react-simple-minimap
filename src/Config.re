module Element = Webapi.Dom.Element;
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

let extractRect = node => Element.ofNode(node) |> Js.Option.getExn |> Element.getBoundingClientRect;

let dimensions = (element, selector, width, height) : dimension => {
  let scroll = {
    width: Element.scrollWidth(element),
    height: Element.scrollWidth(element),
    top: Element.scrollTop(element),
    left: Element.scrollLeft(element)
  };
  let rectange = Element.getBoundingClientRect(element);
  let nodes = Element.querySelectorAll(selector, element);
  let ratioX = width / scroll.width;
  let ratioY = height / scroll.height;
  let nodeList = Array.map((node) => {
    let rect = extractRect(node);
    let wM = Webapi.Dom.DomRect.width(rect) * ratioX;
    let hM = Webapi.Dom.DomRect.height(rect) * ratioY;
    Js.log(wM);
    ()
    /* const wM = width * ratioX;
    const hM = height * ratioY;
    const xM = (left + scrollLeft - sourceRect.left) * ratioX;
    const yM = (top + scrollTop - sourceRect.top) * ratioY; */
  }, NodeList.toArray(nodes));
  {scroll, rectange, nodes}
};

let intToPx = (num) => string_of_int(num) ++ "px";
