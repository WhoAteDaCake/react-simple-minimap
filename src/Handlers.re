module Element = Webapi.Dom.Element;

module DomRect = Webapi.Dom.DomRect;

let roundedifToF = (a, b) => Maths.Mult.ifToF(a, b) |> Maths.frnd;

type fView = {
  x: float,
  y: float,
  width: float,
  height: float
};

let resync = (source, width, height) => {
  let (rect, scroll) = Utils.elemRectAndScroll(source);
  let scaleX = Maths.Div.iiToF(width, scroll.width);
  let scaleY = Maths.Div.iiToF(height, scroll.height);
  let view = {
    x: roundedifToF(scroll.left, scaleX),
    y: roundedifToF(scroll.top, scaleY),
    width:
      roundedifToF(DomRect.width(rect), scaleX) |> min(float_of_int(width)),
    height:
      roundedifToF(DomRect.height(rect), scaleY) |> min(float_of_int(height))
  };
  Utils.Css.coordsStyle(FloatVals(view.width, view.height, view.y, view.x));
};