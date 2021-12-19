#include <bits/stdc++.h>
using namespace std;
#define nr 100002
#define nr2 1002
#define nr3 18000002
#define nr4 10002

class Graf
{
private:

    Graf()
    {
        this->nr_varfuri = 0;
        this->nr_arce = 0;
        this->afisare = 1;
        Citire_Graf();
    }
    Graf(const int n,const int m,const bool ori,vector<int> muchii[])
    {
        this->nr_varfuri = n;
        this->nr_arce = m;
        this->afisare = 0;
        for (int i=0;i<=n;i++)
        {
            int size_subvect = muchii[i].size();
            for (int j=0;j<size_subvect;j++)
            {
                this->muchii_graf[i].push_back(muchii[i][j]);
                if (ori == 0)
                {
                    this->muchii_graf[muchii[i][j]].push_back(i);
                }
            }
        }
    }
    ~Graf() {}

    int nr_varfuri;
    int nr_arce;
    vector<int> muchii_graf[nr];
    bool afisare;
    static Graf* graf;

    // Initializare graf de la tastatura

    void Citire_Graf()
    {
        cout<<"> Doriti citirea numarului de noduri, numarului de muchii si al muchiilor propriu-zise \n> din fisier (0) sau de la tastatura (1) ?\n\n> ";
        bool r;
        cin>>r;
        if (r == 0)
        {
            string fisier;
            cout<<"\n> Numele fisierului din care vor fi citite datele : ";
            cin>>fisier;
            ifstream fin(fisier);
            fin>>this->nr_varfuri;
            fin>>this->nr_arce;
            cout<<"\n> Graful este orientat (0) sau neorientat (1) ? \n\n> ";
            cin>>r;
            int a,b;
            for (int i=0;i<nr_arce;i++)
            {
                fin>>a>>b;
                muchii_graf[a].push_back(b);
                if (r == 1)
                {
                    muchii_graf[b].push_back(a);
                }
            }
            fin.close();
        }
        else if (r == 1)
        {
            cout<<"\n> Numarul de noduri ale grafului : ";
            int a;
            cin>>a;
            this->nr_varfuri = a;
            cout<<"\n> Numarul de muchii ale grafului : ";
            cin>>a;
            this->nr_arce = a;
            int r;
            cout<<"\n> Graful este orientat (0) sau neorientat (1) ? \n\n> ";
            cin>>r;
            cout<<"\n> Pe urmatoarele "<<nr_arce<<" linii introduceti muchiile grafului\n";
            int b;
            for (int i=0;i<nr_arce;i++)
            {
                cout<<"\n> Muchia "<<i+1<<" : ";
                cin>>a>>b;
                this->muchii_graf[a].push_back(b);
                if (r == 1)
                {
                    this->muchii_graf[b].push_back(a);
                }
            }
        }
        cout<<"\n\n";
    }

    // Support functions

    void DF(const int i,bool vizitat[])
    {
        vizitat[i] = true;
        if (this->afisare == 1)
        {
            cout<<i<<' ';
        }
        int size_arce = muchii_graf[i].size();
        for (int j=0;j<size_arce;j++)
        {
            int nod_vec = muchii_graf[i][j];
            if (!vizitat[nod_vec])
                DF(nod_vec,vizitat);
        }
    }

    void DF2(const int n,const int fn,const int number,vector<int> arce[],int dfn[],int low[],stack<pair<int,int>> &Stiva,vector <vector <int>> &Comp)
    {
        dfn[n] = low[n] = number;
        int size_vec = arce[n].size();
        for (int i=0;i<size_vec;i++)
        {
            int vec_crt = arce[n][i];
            if (vec_crt == fn) continue; // Doresc sa ma duc doar in fii lui n, nu inapoi in tatal sau
            if (dfn[vec_crt] == -1)
            {
                Stiva.push(make_pair(n,vec_crt)); // Pun muchia pe stiva
                DF2(vec_crt,n,number+1,arce,dfn,low,Stiva,Comp); // Parcurg mai departe in adancime
                low[n] = min(low[n],low[vec_crt]);
                if (low[vec_crt] >= dfn[n])
                    Add_Comp(n,vec_crt,Stiva,Comp); // Am gasit un nod de articulatie => Introducem componenta biconexa si
                                                    // scoatem de pe stiva muchiile care fac parte din ea
            }
            else low[n] = min(low[n],dfn[vec_crt]);
        }
    }

    void Add_Comp(const int x,const int y,stack<pair<int,int>> &Stiva,vector <vector <int>> &Comp)
    {
        vector<int> Comp_bcnx;
        int nx,ny;
        do
        {
            nx = Stiva.top().first;
            ny = Stiva.top().second;
            Stiva.pop();
            Comp_bcnx.push_back(nx);
            Comp_bcnx.push_back(ny);

        }while(nx != x || ny != y); // Scoatem inclusiv muchia X Y
        Comp.push_back(Comp_bcnx);
    }

    void Vizita(const int n,bool vizitat[],const vector <int> arce[],stack <int> &L) // DFS-ul grafului ( Vizitat false -> true )
    {
        if (!vizitat[n])
        {
            vizitat[n] = true;
            int size_n = arce[n].size();
            for (int i=0;i<size_n;i++)
            {
                Vizita(arce[n][i],vizitat,arce,L);
            }
            L.push(n);
        }
    }

    void Asignare(const int n,stack <int> &L,bool vizitat[],const vector <int> arce_transpuse[],int &nr_comp,vector<int> Comp[]) // DFS-ul grafului transpus ( Vizitat true -> false )
    {
        vizitat[n] = false;
        Comp[nr_comp].push_back(n);

        int size_n = arce_transpuse[n].size();
        for (int i=0;i<size_n;i++)
        {
            int nod_vcn = arce_transpuse[n][i];
            if (vizitat[nod_vcn])
            {
                Asignare(nod_vcn,L,vizitat,arce_transpuse,nr_comp,Comp);
            }
        }
    }

    void DF3(const int n,bool vizitat[],int dfn[],int low[],const vector <int> arce[],vector<pair<int,int>> &muchii_critice)
    {
        vizitat[n] = 1;
        low[n] = dfn[n];

        int size_n = arce[n].size();
        for (int i=0;i<size_n;i++)
        {
            int nr_vcn = arce[n][i];
            if (!vizitat[nr_vcn])
            {
                dfn[nr_vcn] = dfn[n] + 1;
                DF3(nr_vcn,vizitat,dfn,low,arce,muchii_critice);

                low[n] = min(low[n],low[nr_vcn]);
                if (low[nr_vcn] >= dfn[n])
                {
                    muchii_critice.push_back(make_pair(n,nr_vcn));
                }
            }
            else if (dfn[nr_vcn] < dfn[n] - 1)
            {
                low[n] = min(low[n],dfn[nr_vcn]);
            }
        }
    }

    void Update_Cost(const int n,const vector <pair<int,int>> muchii[],int costuri[],int tati[])
    {
        int n_size = muchii[n].size();
        for (int i=0;i<n_size;i++)
        {
            int vecin = muchii[n][i].first;
            int cost = muchii[n][i].second;
            if (costuri[vecin] >= cost)
            {
                costuri[vecin] = cost;
                tati[vecin] = n;
            }
        }
    }

    void Shift_Up(int n,int heap[],const int costuri[],int poz_in_heap[])
    {
        while (n/2 && costuri[heap[n]] < costuri[heap[n/2]])
        {
            swap(heap[n],heap[n/2]);
            swap(poz_in_heap[heap[n]],poz_in_heap[heap[n/2]]);
            n/=2;
        }
    }

    void Shift_Down(int n,int heap[],const int costuri[],int poz_in_heap[],const int heap_lenght)
    {
        while ((n*2 <= heap_lenght && costuri[heap[n]] > costuri[heap[n*2]])||(n*2+1 <= heap_lenght && costuri[heap[n]] > costuri[heap[n*2+1]]))
        {
            if (costuri[heap[n*2]] < costuri[heap[n*2+1]] || n*2+1 > heap_lenght)
            {
                swap(heap[n],heap[n*2]);
                swap(poz_in_heap[heap[n]],poz_in_heap[heap[n*2]]);
                n*=2;
            }
            else
            {
                swap(heap[n],heap[n*2+1]);
                swap(poz_in_heap[heap[n]],poz_in_heap[heap[n*2+1]]);
                n = n*2+1;
            }
        }
    }

    void Update_Cost2(const int n,const vector <pair<int,int>> muchii[],int costuri[],int tati[])
    {
        int n_size = muchii[n].size();
        for (int i=0;i<n_size;i++)
        {
            int vecin = muchii[n][i].first;
            int cost = muchii[n][i].second;
            if (costuri[vecin] > cost + costuri[n])
            {
                costuri[vecin] = cost + costuri[n];
                tati[vecin] = n;
            }
        }
    }

    void DF4(const int i,bool vizitat[],int distanta[],int &diametru)
    {
        vizitat[i] = false;
        int size_arce = muchii_graf[i].size();
        for (int j=0;j<size_arce;j++)
        {
            int nod_vec = muchii_graf[i][j];
            if (vizitat[nod_vec])
            {
                distanta[nod_vec] = distanta[i] + 1;
                if (distanta[nod_vec] > diametru)
                {
                    diametru = distanta[nod_vec];
                }
                DF4(nod_vec,vizitat,distanta,diametru);
            }
        }
    }

    int BF2(const int capacitate[][nr2],const int flux[][nr2],int parinte[])
    {
        int flow_max[nr_varfuri+2];
        flow_max[1] = nr;
        for (int i=2;i<=nr_varfuri;i++)
        {
            parinte[i] = -1;
        }
        parinte[1] = -2;

        queue<int> coada;
        coada.push(1);
        while (!coada.empty())
        {
            int nod_crt = coada.front();
            coada.pop();

            int vec_size = muchii_graf[nod_crt].size();
            for (int i=0;i<vec_size;i++)
            {
                int vec_crt = muchii_graf[nod_crt][i];
                if (capacitate[nod_crt][vec_crt] - flux[nod_crt][vec_crt] > 0 && parinte[vec_crt] == -1)
                {
                    parinte[vec_crt] = nod_crt;
                    flow_max[vec_crt] = min(flow_max[nod_crt],capacitate[nod_crt][vec_crt] - flux[nod_crt][vec_crt]);
                    if (vec_crt != nr_varfuri)
                    {
                        coada.push(vec_crt);
                    }
                    else
                    {
                        return flow_max[nr_varfuri];
                    }
                }
            }
        }
        return 0;
    }

    int BF3(const int capacitate[][nr4],const int flux[][nr4],int parinte[])
    {
        int flow_max[nr_varfuri+2];
        flow_max[0] = nr;
        for (int i=1;i<=nr_varfuri;i++)
        {
            parinte[i] = -1;
        }
        parinte[0] = -2;

        queue<int> coada;
        coada.push(0);
        while (!coada.empty())
        {
            int nod_crt = coada.front();

            coada.pop();

            int vec_size = muchii_graf[nod_crt].size();
            for (int i=0;i<vec_size;i++)
            {
                int vec_crt = muchii_graf[nod_crt][i];
                if (capacitate[nod_crt][vec_crt] - flux[nod_crt][vec_crt] > 0 && parinte[vec_crt] == -1)
                {
                    parinte[vec_crt] = nod_crt;
                    flow_max[vec_crt] = min(flow_max[nod_crt],capacitate[nod_crt][vec_crt] - flux[nod_crt][vec_crt]);
                    if (vec_crt != nr_varfuri)
                    {
                        coada.push(vec_crt);
                    }
                    else
                    {
                        return flow_max[nr_varfuri];
                    }
                }
            }
        }
        return 0;
    }

    void DF5(const int nod_crt,const int nr_noduri,const int cost_crt,bool vizitat[],int &cost_minim,vector<int> costuri[])
    {
        vizitat[nod_crt] = 1;
        int vec_size = muchii_graf[nod_crt].size();

        for (int i=0;i<vec_size;i++)
        {
            int vec_crt = muchii_graf[nod_crt][i];
            int cost_nou = cost_crt + costuri[nod_crt][i];

            if (vizitat[vec_crt] == 0)
            {
                DF5(vec_crt,nr_noduri+1,cost_nou,vizitat,cost_minim,costuri);
            }

            if (nr_noduri == nr_varfuri) // Daca lantul curent include toate nodurile
            {
                if (muchii_graf[nod_crt][i] == 0) // Daca exista muchia de la ultimul element al lantului la primul
                {
                    cost_minim = min(cost_minim,cost_nou);
                }
            }
        }

        vizitat[nod_crt] = 0;
    }

public:

    static Graf *GetGraf();
    static Graf *GetGraf(const int,const int,const bool ori,vector<int>[]);
    static Graf *GetGraf(const int,const int,const bool ori);

    // BFS

    void BFS(int sursa = 1)
    {
        int distanta[nr];
        ofstream fout("bfs.out");

        bool vizitat[nr_varfuri+2];
        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        for (int i=0;i<this->nr_varfuri;i++)
        {
            distanta[i] = -1;
        }

        queue<int> coada;
        coada.push(sursa);
        vizitat[sursa] = true;
        distanta[sursa] = 0;

        if (this->afisare == 1)
        {
            cout<<"\n> Ordinea BFS : ";
        }
        while (!coada.empty())
        {
            int nod_crt = coada.front();
            if (this->afisare == 1)
            {
                cout<<nod_crt<<' ';
            }
            coada.pop();
            int size_arce = muchii_graf[nod_crt].size();
            for (int i=0;i<size_arce;i++)
            {
                int nod_vec = muchii_graf[nod_crt][i];
                if (!vizitat[nod_vec])
                {
                    coada.push(nod_vec);
                    vizitat[nod_vec] = true;
                    distanta[nod_vec] = distanta[nod_crt] + 1;
                }
            }
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Distante de la sursa la fiecare nod : ";
        }
        for (int i=1;i<=this->nr_varfuri;i++)
        {
            fout<<distanta[i]<<' ';
            if (this->afisare == 1)
            {
                cout<<distanta[i]<<' ';
            }
        }
        if (this->afisare == 1)
        {
            cout<<"\n\n";
        }
        fout.close();
    }

    // DFS

    void DFS(int sursa = 1)
    {

        int nr_comp_conexe = 0;
        ofstream fout("dfs.out");
        bool vizitat[nr_varfuri+2];

        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Ordinea DFS : ";
        }
        for (int i=1;i<=this->nr_varfuri;i++)
        {
            if (!vizitat[i])
            {
                nr_comp_conexe++;
                DF(i,vizitat);
            }
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Nr. componente conexe : "<<nr_comp_conexe<<"\n\n";
        }
        fout<<nr_comp_conexe;

        fout.close();
    }

    // Biconex

    void Biconex()
    {

        vector <vector <int>> Comp; // Variabila in care stochez componentele biconexe
        stack <pair <int,int>> Stiva;

        ifstream fin("biconex.in");
        ofstream fout("biconex.out");

        fin>>this->nr_varfuri>>this->nr_arce;

        int a,b;
        vector <int> arce[nr_varfuri+1];
        int dfn[nr_varfuri+1],low[nr_varfuri+1];
        for (int i=1;i<=nr_varfuri;i++)
        {
            dfn[i] = -1; // Nivel
            low[i] = -1; // Nivel minim
        }
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b;
            arce[a].push_back(b); // Graf neorientat
            arce[b].push_back(a);
        }

        DF2(1,0,0,arce,dfn,low,Stiva,Comp);

        int size_comp = Comp.size();
        fout<<size_comp<<"\n";
        for (int i=0;i<size_comp;i++)
        {
            sort(Comp[i].begin(),Comp[i].end());  // Sort + Erase sunt utilizate fiindca la punerea in Comp, au fost puse
            Comp[i].erase(unique(Comp[i].begin(),Comp[i].end()), Comp[i].end());  // extremitatiile muchiilor utilizate,astfel creeandu-se duplicate
            int size_comp_i = Comp[i].size();
            for (int j=0;j<size_comp_i;j++)
                fout<<Comp[i][j]<<' ';
            fout<<"\n";
        }

        fin.close();
        fout.close();
    }

    // Comp Tare Conexe

    void TareConexe()
    {
        ifstream fin("ctc.in");
        ofstream fout("ctc.out");

        fin>>this->nr_varfuri>>this->nr_arce;

        bool vizitat[nr_varfuri+2];
        vector <int> Comp[nr_varfuri+2];
        stack <int> L; // Stochez nodurile in ordinea in care au fost parcurse in DFS-ul original
        int nr_comp = 0; // Nr. comp tari conexe

        for (int i=0;i<=nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        vector <int> arce[nr_varfuri+1];
        vector <int> arce_transpuse[nr_varfuri+1];

        int a,b;
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b;
            arce[a].push_back(b);
            arce_transpuse[b].push_back(a);
        }


        for (int i=1;i<=nr_varfuri;i++)
        {
            Vizita(i,vizitat,arce,L);
        }

        while (!L.empty()) // Iau nodurile in ordinea parcurgerii DFS
        {
            int nr_crt = L.top();
            if (vizitat[nr_crt])
            {
                nr_comp++;
                Asignare(nr_crt,L,vizitat,arce_transpuse,nr_comp,Comp); // Colorezi toti vecinii sai la fel
            }
            L.pop();
        }

        fout<<nr_comp<<"\n";

        for (int i=1;i<=nr_comp;i++)
        {
            sort(Comp[i].begin(),Comp[i].end()); // Fara sortare obtin 90 de puncte
            int size_comp = Comp[i].size();
            for (int j=0;j<size_comp;j++)
                fout<<Comp[i][j]<<' ';
            fout<<"\n";
        }

        fin.close();
        fout.close();
    }

    // Havel - Hakimi

    void Hakimi()
    {
        ifstream fin("hakimi.in");
        ofstream fout("hakimi.out");

        fin>>nr_varfuri;

        vector <int> scv_grade;
        int sum = 0;
        bool ver = true;

        int grad_crt;
        for (int i=0;i<nr_varfuri;i++)
        {
            fin>>grad_crt;
            scv_grade.push_back(grad_crt);

            if (ver == true && grad_crt > nr_varfuri -1 ) // Conditii necesare dar nu suficiente
            {
                fout<<"Nu";
                ver = false;
            }
            sum += grad_crt;
        }

        if (ver == true && sum % 2)
        {
            fout<<"Nu";
            ver = false;
        }

        if (ver == true)
        {
            int k = 0;

            while (k == 0)
            {
                sort(scv_grade.begin(),scv_grade.end(),greater<int>());

                k = 1;

                if (scv_grade[0] != 0)
                {
                    k = 0;
                    for (int i=1;i<=scv_grade[0];i++)
                    {
                        scv_grade[i]--;
                    }
                    scv_grade[0] = 0;
                }

            }

            for (int i=0;i<nr_varfuri;i++)
            {
                cout<<scv_grade[i]<<' ';
            }

            for (int i=0;i<nr_varfuri && ver == true;i++)
            {
                if (scv_grade[i] < 0)
                {
                    fout<<"Nu";
                    ver = false;
                }
            }

            if (ver == true)
            {
                fout<<"Da";
            }
        }

        fin.close();
        fout.close();
    }

    // Muchii Critice

    void PctCritice()
    {
        ifstream fin("pctcritice.in");
        ofstream fout("pctcritice.out");

        fin>>this->nr_varfuri>>this->nr_arce;
        bool vizitat[nr_varfuri+2];

        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        int a,b;
        vector <int> arce[nr_varfuri+2];
        vector <pair<int,int>> muchii_critice;
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b;
            arce[a].push_back(b);
            arce[b].push_back(a);
        }

        int dfn[nr_varfuri+1],low[nr_varfuri+1];
        for (int i=1;i<=nr_varfuri;i++)
        {
            dfn[i] = -1;
            low[i] = -1;
        }

        dfn[1] = low[1] = 0;

        DF3(1,vizitat,dfn,low,arce,muchii_critice);

        int size_v = muchii_critice.size();
        for (int i=0;i<size_v;i++)
        {
            fout<<muchii_critice[i].first<<' '<<muchii_critice[i].second<<"\n";
        }

        fin.close();
        fout.close();
    }

    // Sortare topologica

    void SrtTop()
    {
        ifstream fin("sortaret.in");
        ofstream fout("sortaret.out");

        int gradi[nr_varfuri+2]; // Grad intern

        for (int i=1;i<=nr_varfuri;i++)
        {
            gradi[i] = 0;
        }

        for (int i=1;i<=nr_varfuri;i++)
        {
            int size_nod = muchii_graf[i].size();
            for (int j=0;j<size_nod;j++)
            {
                int vecin = muchii_graf[i][j];
                gradi[vecin]++;
            }
        }

        queue <int> Coada;
        for (int i=1;i<=nr_varfuri;i++)
        {
            if (gradi[i] == 0)
            {
                Coada.push(i);
                gradi[i] = -1;
            }
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Sortare Topologica : ";
        }
        while (!Coada.empty())
        {
            int nr_crt = Coada.front();
            if (this->afisare == 1)
            {
                cout<<nr_crt<<' ';
            }
            fout<<nr_crt<<' ';

            int size_nr = muchii_graf[nr_crt].size();
            for (int i=0;i<size_nr;i++)
            {
                int vcn_crt = muchii_graf[nr_crt][i];
                gradi[vcn_crt]--;
                if (gradi[vcn_crt] == 0)
                    Coada.push(vcn_crt);
            }

            Coada.pop();
        }
        if (this->afisare == 1)
        {
            cout<<"\n";
        }

        fout.close();
    }

    // APM - Prim

    void Prim()
    {
        ifstream fin("apm.in");
        ofstream fout("apm.out");

        fin>>this->nr_varfuri>>this->nr_arce;

        vector <pair<int,int>> muchii[nr_varfuri+2];
        vector <pair<int,int>> muchii_util;
        int heap[nr_varfuri+2];
        int poz_in_heap[nr_varfuri+2];
        int heap_lenght = 0;
        int costuri[nr_varfuri+2];
        int tati[nr_varfuri+2];
        int cost_total = 0;
        for (int i=1;i<=nr_varfuri;i++)
        {
            costuri[i] = 1001;
            tati[i] = -1;
        }

        int a,b,c;
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b>>c;
            muchii[a].push_back(make_pair(b,c));
            muchii[b].push_back(make_pair(a,c));
        }

        costuri[1] = 0;
        Update_Cost(1,muchii,costuri,tati);

        for (int i=2;i<=nr_varfuri;i++) // Am ales 1 ca sursa
        {
            heap_lenght++;
            heap[heap_lenght] = i;
            poz_in_heap[i] = heap_lenght;
            Shift_Up(heap_lenght,heap,costuri,poz_in_heap);
        }

        for (int i=2;i<=nr_varfuri;i++)
        {
            int rad_crt = heap[1];
            swap(heap[1],heap[heap_lenght]);
            swap(poz_in_heap[heap[1]],poz_in_heap[heap[heap_lenght]]);
            heap_lenght--;
            Shift_Down(1,heap,costuri,poz_in_heap,heap_lenght);
            poz_in_heap[rad_crt] = -1;

            Update_Cost(rad_crt,muchii,costuri,tati);
            cost_total+=costuri[rad_crt];
            muchii_util.push_back(make_pair(rad_crt,tati[rad_crt]));

            int rad_crt_vec = muchii[rad_crt].size();
            for (int j=0;j<rad_crt_vec;j++)
            {
                int vec_crt = muchii[rad_crt][j].first;
                if (poz_in_heap[vec_crt] != -1)
                    Shift_Up(poz_in_heap[vec_crt],heap,costuri,poz_in_heap);
            }
        }

        int nr_muchii_util = muchii_util.size();
        fout<<cost_total<<"\n"<<nr_muchii_util<<"\n";
        for (int i=0;i<nr_muchii_util;i++)
        {
            fout<<muchii_util[i].first<<" "<<muchii_util[i].second<<"\n";
        }

        fin.close();
        fout.close();
    }

    // Dijkstra

    void Dijkstra()
    {
        ifstream fin("dijkstra.in");
        ofstream fout("dijkstra.out");

        fin>>this->nr_varfuri>>this->nr_arce;

        vector <pair<int,int>> muchii[nr_arce+2];
        int heap[nr_varfuri+2];
        int costuri[nr_varfuri+2];
        int tati[nr_varfuri+2];
        int poz_in_heap[nr_varfuri+2];
        int heap_lenght = 0;

        for (int i=1;i<=nr_varfuri;i++)
        {
            costuri[i] = 1000001;
            tati[i] = -1;
        }

        int a,b,c;
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b>>c;
            muchii[a].push_back(make_pair(b,c));
        }

        costuri[1] = 0;
        poz_in_heap[1] = -1;
        Update_Cost2(1,muchii,costuri,tati);

        for (int i=2;i<=nr_varfuri;i++)
        {
            heap_lenght++;
            heap[heap_lenght] = i;
            poz_in_heap[i] = heap_lenght;
            Shift_Up(heap_lenght,heap,costuri,poz_in_heap);
        }

        for (int i=2;i<=nr_varfuri;i++)
        {
            int rad_crt = heap[1];
            swap(heap[1],heap[heap_lenght]);
            swap(poz_in_heap[heap[1]],poz_in_heap[heap[heap_lenght]]);
            heap_lenght--;
            Shift_Down(1,heap,costuri,poz_in_heap,heap_lenght);
            poz_in_heap[rad_crt] = -1;

            Update_Cost2(rad_crt,muchii,costuri,tati);
            int rad_crt_vec = muchii[rad_crt].size();
            for (int j=0;j<rad_crt_vec;j++)
            {
                int vec_crt = muchii[rad_crt][j].first;
                if (poz_in_heap[vec_crt] != -1)
                    Shift_Up(poz_in_heap[vec_crt],heap,costuri,poz_in_heap);
            }
        }

        for (int i=2;i<=nr_varfuri;i++)
        {
            if (costuri[i] == 1000001)
                fout<<0<<" ";
            else fout<<costuri[i]<<" ";
        }

        fin.close();
        fout.close();
    }

    // Bellman-Ford

    void Bellman()
    {
        ifstream fin("bellmanford.in");
        ofstream fout("bellmanford.out");

        fin>>this->nr_varfuri>>this->nr_arce;
        vector <pair<int,int>> muchii[nr_arce+2];
        queue <int> coada;
        int ver = 0;
        int costuri[nr_varfuri+2];

        for (int i=1;i<=nr_varfuri;i++)
        {
            costuri[i] = 1000001;
            coada.push(i);
        }

        int a,b,c;
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b>>c;
            muchii[a].push_back(make_pair(b,c));
        }

        costuri[1] = 0;
        int contor = 0;
        while (!coada.empty())
        {
            int i = coada.front();
            int i_size = muchii[i].size();
            for (int j=0;j<i_size;j++)
            {
                int vecin = muchii[i][j].first;
                int cost = muchii[i][j].second;
                if (costuri[vecin] > costuri[i] + cost)
                {
                    coada.push(vecin);
                    costuri[vecin] = costuri[i]+cost;
                }
            }
            coada.pop();
            contor++;
            if (contor == nr_varfuri*nr_arce && !coada.empty())
            {
                ver = 1;
                break;
            }
        }

        if (ver == 0)
        {
            for (int i=2;i<=nr_varfuri;i++)
                fout<<costuri[i]<<" ";
        }
        else fout<<"Ciclu negativ!\n";

        fin.close();
        fout.close();
    }

    // Disjoint

    void Disjoint()
    {
        ifstream fin("disjoint.in");
        ofstream fout("disjoint.out");

        fin>>nr_varfuri>>nr_arce;

        int padure[nr_varfuri+2][3];

        for (int i=1;i<=nr_varfuri;i++)
        {
            padure[i][0] = i; // Parent
            padure[i][1] = 1; // Size
        }

        for (int i=1;i<=nr_arce;i++)
        {
            int a,b,c;
            fin>>a>>b>>c;

            b = padure[b][0];
            while (b!=padure[b][0])
                b = padure[b][0];
            c = padure[c][0];
            while (c!=padure[c][0])
                c = padure[c][0];

            if (a == 1)
            {
                if (b != c)
                {
                    if (padure[b][1] < padure[c][1])
                        swap(b,c);

                    padure[c][0] = b;
                    padure[b][1] += padure[c][1];
                }
            }
            else
            {
                if (b == c)
                    fout<<"DA\n";
                else fout<<"NU\n";
            }
        }

        fin.close();
        fout.close();
    }

    // Darb

    int Darb()
    {
        ifstream fin("darb.in");
        ofstream fout("darb.out");

        bool vizitat[nr_varfuri+2];
        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }
        int distanta[nr_varfuri+2];

        int a,b;
        for (int i=0;i<nr_arce;i++)
        {
            fin>>a>>b;
        }

        queue<int> coada;
        int ultimul;
        coada.push(1);
        vizitat[1] = true;

        while (!coada.empty())
        {
            int nod_crt = coada.front();
            coada.pop();
            int size_arce = muchii_graf[nod_crt].size();
            for (int i=0;i<size_arce;i++)
            {
                int nod_vec = muchii_graf[nod_crt][i];
                if (!vizitat[nod_vec])
                {
                    coada.push(nod_vec);
                    vizitat[nod_vec] = true;
                    ultimul = nod_vec;
                }
            }
        }

        distanta[ultimul] = 1;
        int diametru = 0;
        DF4(ultimul,vizitat,distanta,diametru);
        fout<<diametru;

        fin.close();
        fout.close();
        return diametru;
    }

    // Roy-Floyd

    void Roy()
    {
        ifstream fin("royfloyd.in");
        ofstream fout("royfloyd.out");

        int distanta[nr_varfuri+2][nr_varfuri+2];
        for (int i=0;i<nr_varfuri;i++)
        {
            for (int j=0;j<nr_varfuri;j++)
            {
                int dist;
                fin>>dist;
                if (dist == 0 && i!=j)
                {
                    distanta[i][j] = 1001;
                }
                else
                {
                    distanta[i][j] = dist;
                }
            }
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Matricea de distante initiala : \n\n";
        }

        for (int i=0;i<nr_varfuri;i++)
        {
            if (this->afisare == 1)
            {
                cout<<"> ";
            }
            for (int j=0;j<nr_varfuri;j++)
            {
                if (distanta[i][j] == 1001)
                {
                    cout<<0<<' ';
                    fout<<0<<' ';
                }
                else
                {
                    fout<<distanta[i][j]<<' ';
                    if (this->afisare == 1)
                    {
                        cout<<distanta[i][j]<<' ';
                    }
                }
            }
            fout<<"\n";
            if (this->afisare == 1)
            {
                cout<<"\n";
            }
        }


        for (int k=0;k<nr_varfuri;k++)
        {
            for (int i=0;i<nr_varfuri;i++)
            {
                for (int j=0;j<nr_varfuri;j++)
                {
                    if (i == j) continue;
                    distanta[i][j] = min(distanta[i][j],distanta[i][k]+distanta[k][j]);
                }
            }
        }

        if (this->afisare == 1)
        {
            cout<<"\n> Matricea de distante finala : \n\n";
        }

        for (int i=0;i<nr_varfuri;i++)
        {
            if (this->afisare == 1)
            {
            cout<<"> ";
            }
            for (int j=0;j<nr_varfuri;j++)
            {
                if (distanta[i][j] == 1001)
                {
                    if (this->afisare == 1)
                    {
                        cout<<0<<' ';
                    }
                    fout<<0<<' ';
                }
                else
                {
                    fout<<distanta[i][j]<<' ';
                    if (this->afisare == 1)
                    {
                        cout<<distanta[i][j]<<' ';
                    }
                }
            }
            fout<<"\n";
            if (this->afisare == 1)
            {
                cout<<"\n";
            }
        }

        fin.close();
        fout.close();
    }

    // Max Flow

    int Flow(int flux[][nr2],int capacitate[][nr2])
    {

        int flow = 0;

        int parinte[nr_varfuri+2];
        while(true)
        {
            int flow_max = BF2(capacitate,flux,parinte);
            if (flow_max == 0)
            {
                break;
            }

            flow += flow_max;
            int i = nr_varfuri;
            while (i != 1)
            {
                int vec = parinte[i];
                flux[i][vec] -= flow_max;
                flux[vec][i] += flow_max;
                i = vec;
            }
        }

        return flow;
    }

    // Ciclu Hamiltonian

    int CHam(vector<int> costuri[])
    {
        ofstream fout("hamilton.out");
        int cost = nr3;
        bool vizitat[nr_varfuri+1];
        for (int i=0;i<nr_varfuri;i++)
        {
            vizitat[i] = 0;
        }

        DF5(0,1,0,vizitat,cost,costuri); // Nod de plecare, nr. noduri in ciclu, costul lantului curent

        if (cost != nr3)
        {
            fout<<cost;
        }
        else
        {
            fout<<"Nu exista solutie";
        }

        fout.close();
        return cost;
    }

    // Ciclu Eulerian

    vector<int> CEul(int extr_muchii[][2],int muchii_vizitate[])
    {
        vector<int> raspuns;

        for (int i=1;i<=nr_varfuri;i++)  // Leonhard Euler
        {
            int vec_size = muchii_graf[i].size();
            if (vec_size % 2 == 1)
            {
                raspuns.push_back(-1);
                return raspuns;
            }
        }

        vector<int> stiva; // Stiva manuala pentru a evita stack overflow
        stiva.push_back(1);

        while(!stiva.empty())
        {
            int nod_crt = stiva.back();
            if (muchii_graf[nod_crt].size() > 0)
            {
                int muchie_crt = muchii_graf[nod_crt].back();
                muchii_graf[nod_crt].pop_back();

                if (muchii_vizitate[muchie_crt] == 0) /* Pentru ca muchia x-y este luata o data cand nod_crt = x si apoi cand nod_crt = y,
                                                         fiind stearsa la un pas doar din lista unui singur nod. */
                {
                    muchii_vizitate[muchie_crt] = 1;
                    if (extr_muchii[muchie_crt][0] == nod_crt)
                    {
                        stiva.push_back(extr_muchii[muchie_crt][1]);
                    }
                    else
                    {
                        stiva.push_back(extr_muchii[muchie_crt][0]);
                    }
                }
            }
            else
            {
                stiva.pop_back();
                raspuns.push_back(nod_crt);
            }
        }

        return raspuns;
    }

    // Cuplaj Maxim - Flow modificat

    int Cuplaj(int flux[][nr4],int capacitate[][nr4])
    {
        int flow = 0;

        int parinte[nr_varfuri+2];
        while(true)
        {
            int flow_max = BF3(capacitate,flux,parinte);
            if (flow_max == 0)
            {
                break;
            }
            flow += flow_max;
            int i = nr_varfuri;
            while (i != 0)
            {
                int vec = parinte[i];
                flux[i][vec] -= flow_max;
                flux[vec][i] += flow_max;
                i = vec;
            }
        }

        return flow;
    }

};

Graf *Graf::graf = nullptr;
Graf *Graf::GetGraf()
{
    if (graf == nullptr)
        graf = new Graf();

    return graf;
}

Graf *Graf::GetGraf(const int n,const int m,const bool ori,vector<int> muchii[])
{
    if (graf == nullptr)
        graf = new Graf(n,m,ori,muchii);

    return graf;
}

void Rezultat_General() // Utilizat pentru majoritatea problemelor Infoarena unde se dau n,m si lista de muchii
{
    ifstream fin("bfs.in");
    int n,m;
    fin>>n>>m;

    vector<int> muchii[nr2];
    int a,b;
    for (int i=0;i<m;i++)
    {
        fin>>a>>b;
        muchii[a].push_back(i);
    }

    Graf::GetGraf(n,m,1,muchii)->BFS(2); // Noduri, Muchii, Orientat (1/0), Vector de muchii

    fin.close();
}

void Rezultat_MaxFlow()
{
    ifstream fin("maxflow.in");
    ofstream fout("maxflow.out");
    int n,m;
    fin>>n>>m;

    vector<int> muchii[nr2];
    int flux[n+2][nr2];
    int capacitate[n+2][nr2];
    int x,y,z;
    for (int i=0;i<m;i++)
    {
        fin>>x>>y>>z;
        capacitate[x][y] = z;
        flux[x][y] = 0;
        flux[y][x] = 0;
        muchii[x].push_back(y);
        muchii[y].push_back(x);
    }

    int raspuns = Graf::GetGraf(n,m,1,muchii)->Flow(flux,capacitate);
    fout<<raspuns;

    fin.close();
    fout.close();
}

void Rezultat_CHam()
{
    ifstream fin("hamilton.in");
    int n,m;
    fin>>n>>m;

    vector<int> muchii[nr2], costuri[nr2];
    int a,b,c;
    for (int i=0;i<m;i++)
    {
        fin>>a>>b>>c;
        muchii[a].push_back(b);
        costuri[a].push_back(c);
    }

    fin.close();
    Graf::GetGraf(n,m,1,muchii)->CHam(costuri);
}

void Rezultat_CEul()
{
    ifstream fin("ciclueuler.in");
    ofstream fout("ciclueuler.out");
    int n,m;
    fin>>n>>m;

    vector<int> muchii[n+2];
    int extr_muchii[m+2][2];  // Nodurile intre care se afla muchia
    int muchii_vizitate[m+2]; // Flag pentru muchiile utilizate in ciclu
    int a,b;
    for (int i=0;i<m;i++)
    {
        fin>>a>>b;
        muchii[a].push_back(i);
        muchii[b].push_back(i);
        extr_muchii[i][0] = a;
        extr_muchii[i][1] = b;
        muchii_vizitate[i] = 0;
    }

    vector<int> raspuns = Graf::GetGraf(n,m,1,muchii)->CEul(extr_muchii,muchii_vizitate); // Noduri, Muchii, Orientat (1/0), Vector de muchii
    int raspuns_size = raspuns.size();
    if (raspuns_size > 1) // Daca exista un ciclu, nu dorim afisarea nodului initial la finalul ciclului
    {
        raspuns_size--;
    }

    for (int i=0;i<raspuns_size;i++)
    {
        fout<<raspuns[i]<<' ';
    }

    fin.close();
    fout.close();
}

void Rezultat_Cuplaj() // 30 Pct Infoarena -> Raspuns corect la toate testele, Timpul de executie mult peste limita
{
    ifstream fin("cuplaj.in");
    ofstream fout("cuplaj.out");
    int n,m,e;
    fin>>n>>m>>e; // n = L , m = R, e = Muchii
    vector<int> muchii[nr4*2]; // Elementele L sunt 1->n iar R sunt (n+1)->(n+m)
    int capacitate[n+m+10][nr4];
    int flux[n+m+10][nr4];
    int x,y;
    for (int i=0;i<e;i++)
    {
        fin>>x>>y;
        flux[x][n+y] = 0;
        flux[y][n+x] = 0;
        capacitate[x][n+y] = 1;
        muchii[x].push_back(n+y);
        muchii[n+y].push_back(x);
    }

    // Trebuie adaugate sursa si destinatia. Sursa o vom pune 0 (nodurile incep de la 1) iar destinatia n+m+1
    for (int i=1;i<=n;i++)
    {
        muchii[0].push_back(i);
        muchii[i].push_back(0);
        capacitate[0][i] = 1;
        flux[0][i] = 0;
        flux[i][0] = 0;
    }
    for (int i=1;i<=m;i++)
    {
        muchii[n+m+1].push_back(n+i);
        muchii[n+i].push_back(n+m+1);
        capacitate[n+i][n+m+1] = 1;
        flux[n+m+1][n+i] = 0;
        flux[n+i][n+m+1] = 0;
    }

    int raspuns = Graf::GetGraf(n+m+1,e,1,muchii)->Cuplaj(flux,capacitate);
    fout<<raspuns<<"\n";

    for (int i=1;i<=n;i++)
    {
        for (int j=1;j<=m;j++)
        {
            if (flux[i][n+j] == 1)
            {
                fout<<i<<' '<<j<<"\n";
            }
        }
    }

    fin.close();
    fout.close();
}

void Meniu()
{
    meniu:
    system("CLS");
    cout<<"  ____________________\n";
    cout<<" |       Meniu       |\n";
    cout<<" |___________________|\n";
    cout<<" |      (1) BFS      |\n";
    cout<<" |___________________|\n";
    cout<<" |      (2) DFS      |\n";
    cout<<" |___________________|\n";
    cout<<" |    (3) Biconex    |\n";
    cout<<" |___________________|\n";
    cout<<" |      (4) CTC      |\n";
    cout<<" |___________________|\n";
    cout<<" | (5) Havel-Hakimi  |\n";
    cout<<" |___________________|\n";
    cout<<" |  (6) Pct Critice  |\n";
    cout<<" |___________________|\n";
    cout<<" |   (7) Sort Top    |\n";
    cout<<" |___________________|\n";
    cout<<" |  (8) APM - Prim   |\n";
    cout<<" |___________________|\n";
    cout<<" |  (9) Dijkstra     |\n";
    cout<<" |___________________|\n";
    cout<<" | (10) Bellman-Ford |\n";
    cout<<" |___________________|\n";
    cout<<" |  (11) Disjoint    |\n";
    cout<<" |___________________|\n";
    cout<<" |    (12) Darb      |\n";
    cout<<" |___________________|\n";
    cout<<" |  (13) Roy-Floyd   |\n";
    cout<<" |___________________|\n";
    cout<<" |  (14) Max Flow    |\n";
    cout<<" |___________________|\n";
    cout<<" |    (15) Exit      |\n";
    cout<<" |___________________|\n";

    int Raspuns;
    jump:
    cout<<"\n> ";
    cin>>Raspuns;

    if (Raspuns == 1)
    {
        int sursa;
        cout<<"\n> Care sa fie punctul de plecare al BFS-ului? \n> ";
        cin>>sursa;
        Graf::GetGraf()->BFS(sursa);
        cout<<"\n> Raspunsul a fost exportat si in bfs.out";
    }
    else if (Raspuns == 2)
    {
        int sursa;
        cout<<"\n> Care sa fie punctul de plecare al DFS-ului? \n> ";
        cin>>sursa;
        Graf::GetGraf()->DFS(sursa);
        cout<<"\n> Raspunsul a fost exportat si in dfs.out";
    }
    else if (Raspuns == 3)
    {
        Graf::GetGraf()->Biconex();
        cout<<"\n> Datele de intrare au fost preluate din biconex.in";
        cout<<"\n> Raspunsul a fost exportat si in biconex.out";
    }
    else if (Raspuns == 4)
    {
        Graf::GetGraf()->TareConexe();
        cout<<"\n> Datele de intrare au fost preluate din ctc.in";
        cout<<"\n> Raspunsul a fost exportat si in ctc.out";
    }
    else if (Raspuns == 5)
    {
        Graf::GetGraf()->Hakimi();
        cout<<"\n> Datele de intrare au fost preluate din hakimi.in";
        cout<<"\n> Raspunsul a fost exportat si in hakimi.out";
    }
    else if (Raspuns == 6)
    {
        Graf::GetGraf()->PctCritice();
        cout<<"\n> Datele de intrare au fost preluate din pctcritice.in";
        cout<<"\n> Raspunsul a fost exportat si in pctcritice.out";
    }
    else if (Raspuns == 7)
    {
        Graf::GetGraf()->SrtTop();
        cout<<"\n> Raspunsul a fost exportat si in sortaret.out";
    }
    else if (Raspuns == 8)
    {
        Graf::GetGraf()->Prim();
        cout<<"\n> Datele de intrare au fost preluate din apm.in";
        cout<<"\n> Raspunsul a fost exportat si in apm.out";
    }
    else if (Raspuns == 9)
    {
        Graf::GetGraf()->Dijkstra();
        cout<<"\n> Datele de intrare au fost preluate din dijkstra.in";
        cout<<"\n> Raspunsul a fost exportat si in dijkstra.out";
    }
    else if (Raspuns == 10)
    {
        Graf::GetGraf()->Bellman();
        cout<<"\n> Datele de intrare au fost preluate din bellmanford.in";
        cout<<"\n> Raspunsul a fost exportat si in bellmanford.out";
    }
    else if (Raspuns == 11)
    {
        Graf::GetGraf()->Disjoint();
        cout<<"\n> Datele de intrare au fost preluate din disjoint.in";
        cout<<"\n> Raspunsul a fost exportat si in disjoint.out";
    }
    else if (Raspuns == 12)
    {

        cout<<"\n> Datele de intrare au fost preluate din darb.in";
        cout<<"\n> Raspunsul este : "<<Graf::GetGraf()->Darb();
        cout<<"\n> Raspunsul a fost exportat si in darb.out";
    }
    else if (Raspuns == 13)
    {
        cout<<"\n> Datele de intrare suplimentare au fost preluate din royfloyd.in";
        Graf::GetGraf()->Roy();
        cout<<"\n> Raspunsul a fost exportat si in royfloyd.out";
    }
    else if (Raspuns == 14)
    {
        cout<<"\n> Datele de intrare suplimentare au fost preluate din maxflow.in";

        cout<<"\n> Raspunsul a fost exportat si in maxflow.out";
    }
    else if (Raspuns == 15)
    {
        exit(0);
    }
    else
    {
        cout<<"\n> Comanda inexistenta\n";
        goto jump;
    }
    cout<<"\n\n> Doriti revenirea la meniu (1) sau iesirea din program (2) ? \n> ";
    cin>>Raspuns;
    if (Raspuns == 1)
    {
        goto meniu;
    }
}


int main()
{
    /*
    // Pentru afisare tastatura + ecran

    Graf::GetGraf();
    Meniu();
    */

    Rezultat_Cuplaj();

    return 0;
}
