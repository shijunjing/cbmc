(declare-const str String)
(declare-const suffix String)
(assert (str.suffixof suffix str))
(check-sat)
