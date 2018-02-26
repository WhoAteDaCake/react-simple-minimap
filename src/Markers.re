module Element = Webapi.Dom.Element;

module NodeList = Webapi.Dom.NodeList;

module DomRect = Webapi.Dom.DomRect;

type markers = array(ReasonReact.reactElement);

type minimapProps = {
  top: int,
  left: int
};

type spec = {
  markers,
  minimapProps
};

let toStyle = (n1, n2) => Maths.Mult.ifToF(n1, n2) |> Utils.Css.floatToPx;

let create = (element, selector, minimapW, minimapH) : spec => {
  let scroll = Utils.elemScroll(element);
  let sourceRect = Element.getBoundingClientRect(element);
  let nodes = Element.querySelectorAll(selector, element);
  let ratioX = Maths.Div.iiToF(minimapW, scroll.width);
  let ratioY = Maths.Div.iiToF(minimapH, scroll.height);
  let markers =
    Array.mapi(
      (i, node) => {
        let markerRect = Utils.nodeToRect(node);
        let width = toStyle(DomRect.width(markerRect), ratioX);
        let height = toStyle(DomRect.height(markerRect), ratioY);
        let left =
          toStyle(
            DomRect.left(markerRect) + scroll.left - DomRect.left(sourceRect),
            ratioX
          );
        let top =
          toStyle(
            DomRect.top(markerRect) + scroll.top - DomRect.top(sourceRect),
            ratioY
          );
        let style = ReactDOMRe.Style.make(~width, ~height, ~left, ~top, ());
        <div key=(string_of_int(i)) style className="minimap-children" />;
      },
      NodeList.toArray(nodes)
    );
  let minimapProps = {
    top: DomRect.top(sourceRect),
    left: DomRect.left(sourceRect) + scroll.width - minimapW - 5
  };
  {markers, minimapProps};
};