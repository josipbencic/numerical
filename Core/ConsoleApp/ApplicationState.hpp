#ifndef INCLUDED_APPLICATION_STATE_HPP
#define INCLUDED_APPLICATION_STATE_HPP

#include <ostream>
#include <vector>

enum class EApplicationState;


class OutputUtil {
public:

  template <typename VectorSpace>
  static void writeGramSchmidtSolutionToStream(std::vector<VectorSpace>& v, std::ostream* stream) {
    std::ostream& out = *stream;
    out << "\n\t The result:\n\n";
    auto ret = math::GramSchmidt(v);
    for (auto& x : ret) {
      out << "\t" << x << "\n";
    }
  }

  template <typename VectorSpace>
  static void writeDistanceFromSubspaceSolutionToStream(std::vector<VectorSpace>& linearHull, std::ostream* stream) {

    std::ostream& out = *stream;
    if (linearHull.empty()) {
      out << "The linear subspace has not been specified properly!" << std::endl;
      return;
    }
    VectorSpace v = linearHull[linearHull.size() - 1];
    linearHull.pop_back();
    double distance = distanceFromSpace(linearHull, v);
    out << "\n\t The distance is:\n\n";
    out << distance << std::endl;
  }

  static void writeLinearSystemSolutionToStream(std::vector<double>& ret, std::ostream* stream) {
    std::ostream& out = *stream;

    out << "\t The result:\n\n";
    for (std::size_t i = 0; i < ret.size(); i++) {
      out << "\tx" << (i + 1) << " = " << ret[i] << "\n";
    }
    out << std::endl;
  }
};

/*  A family of functors used to display states.
    Each class represents a part of application and
    handles certain (name - explainatory) function.
*/

/*  ABC for polymorphism. */
class ApplicationState {
public:
  virtual ~ApplicationState() {}
  virtual EApplicationState operator ()(std::ostream*) = 0;
};

/*  First screen to appear when the application is started.
    A simple greet message that ends with any keystroke.
*/
class WelcomeState : public ApplicationState {
public:
  EApplicationState operator ()(std::ostream* stream = nullptr);
};

/*  Main menu. All Views return to this when they finish.
    It allows to switch all other Views (except WelcomeView).
*/
class MainMenuState : public ApplicationState {
public:
  EApplicationState operator ()(std::ostream* stream = nullptr);
};

/*  One of given options at Main Menu View.
    Gives an overview of input methods for all algorithms.
*/
class TutorialState : public ApplicationState {
public:
  EApplicationState operator ()(std::ostream* stream = nullptr);
};

/*  Base class for all algorithms. Contains an additional
    reference to InputManager class that contains vectors given
    by user.
*/
class AlgorithmState : public ApplicationState {
public:
  AlgorithmState(class InputManager& rManager)
    : input(rManager)
  {}
  virtual EApplicationState operator ()(std::ostream* stream = nullptr) = 0;
protected:
  class InputManager &input;
};

/*  Handles distance from subspace algorithm.
  Sole algorithm is documented in GramSchmidt.hpp file,
  this is only its View.
*/
class DistanceFromSubspaceState : public AlgorithmState {
public:
  DistanceFromSubspaceState(class InputManager& rManager)
    : AlgorithmState(rManager)
  {}

  EApplicationState operator ()(std::ostream* stream = nullptr);
};

/*  Gram Schmidt Orthogonalization Algorithm View.
  Returns an array of solution which are printed and then destroyed.
*/
class GramSchmidtState : public AlgorithmState {
public:
  GramSchmidtState(class InputManager& rManager)
    : AlgorithmState(rManager)
  {}

  EApplicationState operator ()(std::ostream* stream = nullptr);
};

/*  Handles Linear System solver. Also repeats the input tutorial
  regarding Linear Systems.
*/
class LinearSystemState : public AlgorithmState {
public:
  LinearSystemState(class InputManager& rManager)
    : AlgorithmState(rManager)
  {}

  EApplicationState operator ()(std::ostream* stream = nullptr);
};

#endif
