# include "cell_print.h"

void print_cells(int step, rvector<bool>& alive_status, double& fraction)
{
    char alive_char = 'O';
    char dead_char = ' ';
    std::string string_represention(alive_status.size(), ' ');
    for (int i = 0; i < alive_status.size(); i++)
        if (alive_status[i])
            string_represention[i] = alive_char;
        else
            string_represention[i] = dead_char;
    std::cout << step << "\t" << string_represention << " " << fraction << "\n";
}
