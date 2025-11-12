#include <state.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        State& state = State::get_instance();
        while (!state.should_quit())
        {
            state.render();
            state.process_input();
            state.update();
        }
        return EXIT_SUCCESS;

    }

    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}