#include "headers/reader.h"


int main(){
    readFile("data/100/datos-10-N100.txt");
    cout << name << endl;
    cout << "bmax:"  << prog_params.bmax << " cpow:" << prog_params.cpow << " f0char:" << prog_params.f0char << " f0vehicle:" << prog_params.f0vehicle << " nk:" << prog_params.nk << " nn:" << prog_params.nn << " nr:" << prog_params.nr << " ntr:" << prog_params.ntr << " qmax:" << prog_params.qmax << " timemax:" << prog_params.timemax << " vel:" << prog_params.vel << endl;
    cout << c_nodes.size() << endl;
    cout << r_nodes.size() << endl;
    cout << tech_list.size() << endl;
}