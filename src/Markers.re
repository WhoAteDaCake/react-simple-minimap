module Element = Webapi.Dom.Element;

module NodeList = Webapi.Dom.NodeList;

module DomRect = Webapi.Dom.DomRect;

type markers = array(ReasonReact.reactElement);

type minimapProps = {
  top: int,
  left: int
};

let toStyle = (n1, n2) => Maths.Mult.ifToF(n1, n2) |> Utils.Css.floatToPx;

let create = (element, selector, minimapW, minimapH, markerColor) : markers => {
  let (rect, scroll) = Utils.elemRectAndScroll(element);
  let ratioX = Maths.Div.iiToF(minimapW, scroll.width);
  let ratioY = Maths.Div.iiToF(minimapH, scroll.height);
  let markerRects =
    Element.querySelectorAll(selector, element)
    |> NodeList.toArray
    |> Array.map(Utils.nodeToRect);
  let markers =
    Array.mapi(
      (i, markerRect) => {
        let width = toStyle(DomRect.width(markerRect), ratioX);
        let height = toStyle(DomRect.height(markerRect), ratioY);
        let left =
          toStyle(
            DomRect.left(markerRect) + scroll.left - DomRect.left(rect),
            ratioX
          );
        let top =
          toStyle(
            DomRect.top(markerRect) + scroll.top - DomRect.top(rect),
            ratioY
          );
        let style =
          ReactDOMRe.Style.make(
            ~width,
            ~height,
            ~left,
            ~top,
            ~backgroundColor=markerColor,
            ()
          );
        <div key=(string_of_int(i)) style className="minimap-children" />;
      },
      markerRects
    );
  markers;
};