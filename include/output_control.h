extern bool suppressOutput;    // toggled true when tests are being ran

struct SuppressOutputSetter {  // initialized at the top of every test file to turn off idle task output
  SuppressOutputSetter() {
    suppressOutput = true;
  }
};