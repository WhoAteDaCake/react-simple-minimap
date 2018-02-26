module Div = {
  let iiToF = (n1: int, n2: int): float => float_of_int(n1) /. float_of_int(n2);
  let fiToF = (n1: float, n2: int): float => n1 /. float_of_int(n2);
  let ifToF = (n1: int, n2: float): float => float_of_int(n1) /. n2;
};

module Mult = {
  let ifToF = (n1: int, n2: float): float => float_of_int(n1) *. n2;
};