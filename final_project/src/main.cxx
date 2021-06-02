#include "controller.hxx"
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* argv[])

try {

    bool use_stalemate;

    switch(argc) {
    case 1:
        use_stalemate = false;
        break;
    default:
        use_stalemate = true;
        break;
    }

    Controller(use_stalemate).run();
}


catch (exception const& e) {
    cerr << argv[0] << ": " << e.what() << "\n";
    return 1;
}

