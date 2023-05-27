#ifndef CHECKARGS_H
#define CHECKARGS_H

#include <global.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
    const std::string optString = "n:h";
    
    const std::string options = "-n SIZE -s SIMD [-h]";

    const std::string description = "Descripción:\n"
                    "\t-n   Tamaño del problema (exponente en base 2)"
										"\t-s		Habilitar procesamiento vectorial";
	
		typedef struct args_t {
			int32_t SIZE;
			int32_t SIMD;
		} args_t;
	
		args_t parameters;
	
		int argc;
		char **argv;

public:
    checkArgs(int _argc , char **_argv);
    ~checkArgs();
    args_t getArgs();
    
private:
    void printUsage();
};

checkArgs::checkArgs(int _argc , char **_argv) {
	parameters.SIZE = 0;
	parameters.SIMD = false;
	
	argc = _argc;
	argv = _argv;
}

checkArgs::~checkArgs() {
	
}

checkArgs::args_t checkArgs::getArgs(){
	int option;

	while ((option = getopt(argc, argv, (optString + "s").c_str())) != -1){
		switch (option) {
			case 'n':
					parameters.SIZE = atoi(optarg);
					break;
			case 's':
					parameters.SIMD = true;
					break;
			case 'h':
			default:
					printUsage();
					exit(EXIT_FAILURE);
		}
	}

	if (parameters.SIZE <= 0 ||
		 (parameters.SIMD != true && parameters.SIMD != false)) {
	  printUsage();
	  exit(EXIT_FAILURE);
	}
	
	return(parameters);
}

void checkArgs::printUsage() {
	std::cout << "Usage: " << argv[0] << " " << options << " " << description << std::endl;
}

#endif // CHECKARGS_H