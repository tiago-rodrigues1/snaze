#include "snaze.hpp"
#include <stdexcept>
#include <iostream>

RunningOpt SnazeSimulation::run_options;

void SnazeSimulation::usage()
{

    std::cout << "Usage: snaze [<options>] <input_level_file>\n"
              << "Game simulation options:\n"
              << "--help/-h Print this help text.\n"
              << "--fps/-f <num> Number of frames (board) presented per second.\n"
              << "--lives/-l <num> Number of lives the snake shall have. Default = 5.\n"
              << "--food/-d <num> Number of food pellets for the entire simulation. Default = 10.\n"
              << "--playertype/-p <type> Type of snake intelligence: random, backtracking. Default = backtracking\n";
}

bool has_next_argument(int i, int argc)
{
    if (i + 1 >= argc)
    {
        std::cout << "Warning: you need to provide a number or a player type\n";
        return false;
    }
    return true;
}

void assign_if_valid_number(const std::string &str_value, RunningOpt &run_options, int run_options_num)
{
    try
    {
        int num = std::stoi(str_value);

        if (num > 0)
        {
            switch (run_options_num)
            {
            case 0:
                run_options.fps = num;
                break;
            case 1:
                run_options.lives = num;
                break;
            case 2:
                run_options.food = num;
                break;
            default:
                break;
            }
        }
        else{
           std::cout << "Warning: please choose a number bigger than 0\n"; 
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Warning: invalid number\n";
    }
}

void SnazeSimulation::validate_arguments(int argc,  char *argv[], RunningOpt &run_options)
{
    bool skip_next{false};

    for (int i{1}; i < argc; ++i)
    {
        if (skip_next)
        {
            skip_next = false;
            continue;
        }

        std::string current_arg{argv[i]};

        if (current_arg == "--help" || current_arg == "-h")
        {
            usage();
            exit(0);
        }

        else if ((current_arg == "--fps" || current_arg == "-f") && has_next_argument(i, argc))
        {
            assign_if_valid_number(argv[i + 1], run_options, 0);
            skip_next = true;
        }

        else if ((current_arg == "--lives" || current_arg == "-l") && has_next_argument(i, argc))
        {
            assign_if_valid_number(argv[i + 1], run_options, 1);
            skip_next = true;
        }

        else if ((current_arg == "--food" || current_arg == "-d") && has_next_argument(i, argc))
        {
            assign_if_valid_number(argv[i + 1], run_options, 2);
            skip_next = true;
        }

        else if ((current_arg == "--playertype" || current_arg == "-p") && has_next_argument(i, argc))
        {
            std::string next_arg{argv[i + 1]};
            if (next_arg == "random")
            {
                run_options.player_type = player_type_e::RANDOM;
            }
            else if (next_arg == "backtracking")
            {
                run_options.player_type = player_type_e::BACKTRACKING;
            }
            else
            {
                std::cout << "Warning: invalid playertype option\n";
            }
            skip_next = true;
        }
        else
        {
            if(current_arg.substr(0, 2) == "--" || current_arg[0] == '-'){
                std::cout << "Warning: invalid option\n";
            }
            else {

                // validação dos arquivos
            }
        }
    }
}

void SnazeSimulation::initialize(int argc, char *argv[])
{

    if (argc == 1)
    {
        std::cerr << "you need to provide at least one file or directory\n";
        std::exit(1);
    }

    validate_arguments(argc, argv, run_options);
}