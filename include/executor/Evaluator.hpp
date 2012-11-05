#pragma once

template <typename DerivedType>
struct Evaluator {
};

template <typename E1, typename E2>
class EvalPair : public Evaluator<EvalPair<E1,E2>> {
  const E1* e1_;
  const E2* e2_;
public:
  EvalPair(const Evaluator<E1>* e1, const Evaluator<E2>* e2)
  : e1_(static_cast<const E1*>(e1)), e2_(static_cast<const E2*>(e2)) {
  }
  ~EvalPair() {
    delete e1_;
    delete e2_;
  }
  template <typename BoxContext>
  inline void execute(BoxContext& bc) const {
    e1_->execute(bc);
    e2_->execute(bc);
  }
};


template <typename E1, typename E2>
EvalPair<E1,E2>* make_evaluator(const Evaluator<E1>* e1,
                                const Evaluator<E2>* e2) {
  return new EvalPair<E1,E2>(e1, e2);
}

template <typename E1, typename E2, typename E3>
EvalPair<E1,EvalPair<E2,E3>>* make_evaluator(const Evaluator<E1>* e1,
                                             const Evaluator<E2>* e2,
                                             const Evaluator<E3>* e3) {
  return make_evaluator(e1, make_evaluator(e2, e3));
}