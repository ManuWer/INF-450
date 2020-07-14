#include <iostream>
using namespace std;

class Data {

    private:
        int dia;
        int mes;
        int ano;

    public:
        Data (int d = 1, int m = 1, int a = 2018) {
            setDia(d);
            setMes(m);
            setAno(a);
        }
       
        void setDia (int d) {
            if (d<1) d = 1;
            else if (d>30) d = 30;
            dia = d;
        }

        void setMes (int m) {
            if (m<1) m = 1;
            else if (m>12) m = 12;
            mes = m;
        }

        void setAno (int a) {
            if (a<2018) a = 2018;
            else if (a>2020) a = 2020;
            ano = a;
        }

        int getDia() const { return dia; }

        int getMes() const { return mes; }

        int getAno() const { return ano; }
};
      