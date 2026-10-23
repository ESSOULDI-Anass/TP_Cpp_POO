#include <iostream>
#include <vector>


using namespace std;

int main() {


/*
    vector<int> nombres = {10, 20, 30, 40, 50};

    vector<int>::iterator it;

    vector<int>::reverse_iterator it_rev;

    for (it = nombres.begin(); it != nombres.end(); ++it) {
        cout << *it << " ";
    }

      cout<<endl;

    for (it_rev = nombres.rbegin(); it_rev != nombres.rend(); ++it_rev) {
        cout << *it_rev << " ";
    }

    cout<<endl;


    vector<int> nbr_pg{nombres.begin()+1,nombres.begin()+3};

   for (int elem:nbr_pg)
    cout<<elem<<" ";
   cout<<endl;

   vector<int> nbr_inv{nbr_pg.rbegin(),nbr_pg.rend()};

   for(int elem1 : nbr_inv)
    cout<<elem1<<" "
   cout<<endl;

*/

    vector<int> v={1,2,3};
    auto it=v.begin();
     v.insert(v.begin(),0);
     cout<<*it;



    return 0;
}
