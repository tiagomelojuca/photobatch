#include <iostream>
#include "ArgumentParser.h"
#include "Mode.h"

int main(int argc, char* argv[]) {

    ArgumentParser argParser;

    argParser.registerFlag(Args::Flags::help);
    argParser.registerFlag(Args::Flags::rename);
    argParser.registerFlag(Args::Flags::convert);
    argParser.registerFlag(Args::Flags::resize);
    argParser.registerFlag(Args::Flags::scale);

    argParser.registerOption(Args::Options::folder);
    argParser.registerOption(Args::Options::filter);

    argParser.registerOption(Args::Options::width);
    argParser.registerOption(Args::Options::height);

    argParser.registerOption(Args::Options::amount);

    argParser.registerOption(Args::Options::prefix);
    argParser.registerOption(Args::Options::startNumber);

    argParser.registerOption(Args::Options::from);
    argParser.registerOption(Args::Options::to);

    argParser.setHelpMessage(
       R"(USAGE: photobatch --[rename|convert|resize|scale] [options]

       Photobatch comes with 4 operation modes.
       At least one (and no more than one) must be active.
       To activate a mode, use some of following flags:
    
       --rename         rename a set of files
       --scale          scale a set of files
       --convert        convert a set of files
       --resize         resize a set of files)"
    );

    argParser.parse(argc, argv);

    if (argParser.getFlag(Args::Flags::help)) {
        std::cout << argParser.getHelpMessage() << std::endl;
        return 0;
    }

    try {
        if(std::unique_ptr<Mode> currentMode = createMode(argParser)) {
            currentMode->Run();
        }
    } catch (const std::exception& exception) {
        std::cerr << "ERROR: " << exception.what() << std::endl;
        std::cerr << "For more information about the usage, check --help" << std::endl;
    }

    return 0;

}
