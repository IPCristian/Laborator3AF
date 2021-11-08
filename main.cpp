#include <bits/stdc++.h>
using namespace std;
#define nr 100002

class Graf
{
private:

    Graf()
        {
            this->nr_varfuri = 0;
            this->nr_arce = 0;
        }
    ~Graf() {}

    int nr_varfuri;
    int nr_arce;
    static Graf* graf;

public:

    static Graf *GetGraf();

    // BFS

    void BFS()
    {
        int sursa;
        int distanta[nr];

        ifstream fin("bfs.in");
        ofstream fout("bfs.out");

        fin>>this->nr_varfuri>>this->nr_arce>>sursa;

        bool vizitat[nr_varfuri+2];
        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        for (int i=0;i<this->nr_varfuri;i++)
        {
            distanta[i] = -1;
        }

        int a,b;
        vector <int> arce[nr_varfuri+1];
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b;
            arce[a].push_back(b);
        }

        queue<int> coada;
        coada.push(sursa);
        vizitat[sursa] = true;
        distanta[sursa] = 0;

        while (!coada.empty())
        {
            int nod_crt = coada.front();
            coada.pop();
            int size_arce = arce[nod_crt].size();
            for (int i=0;i<size_arce;i++)
            {
                int nod_vec = arce[nod_crt][i];
                if (!vizitat[nod_vec])
                {
                    coada.push(nod_vec);
                    vizitat[nod_vec] = true;
                    distanta[nod_vec] = distanta[nod_crt] + 1;
                }
            }
        }

        for (int i=1;i<=this->nr_varfuri;i++)
            fout<<distanta[i]<<' ';


        fin.close();
        fout.close();

    }

    // DFS

    void DFS()
    {

        int nr_comp_conexe = 0;

        ifstream fin("dfs.in");
        ofstream fout("dfs.out");

        fin>>this->nr_varfuri>>this->nr_arce;
        bool vizitat[nr_varfuri+2];

        for (int i=0;i<=this->nr_varfuri;i++)
        {
            vizitat[i] = false;
        }

        int a,b;
        vector <int> arce[nr_varfuri+1];
        for (int i=0;i<this->nr_arce;i++)
        {
            fin>>a>>b;
            arce[a].push_back(b);
            arce[b].push_back(a);
        }

        for (int i=1;i<=this->nr_varfuri;i++)
        {
            if (!vizitat[i])
            {
                nr_comp_conexe++;
                DF(i,arce,vizitat);
            }
        }

        fout<<nr_comp_conexe;

        fin.close();
        fout.close();
    }

    void DF(int i,vector<int> arce[],bool vizitat[])
    {
        vizitat[i] = true;
        int size_arce = arce[i].size();
        for (int j=0;j<size_arce;j++)
        {
            int nod_vec = arce[i][j];
            if (!vizitat[nod_vec])
                DF(nod_vec,arce,vizitat);
        }
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

    void DF2(int n,int fn,int number,vector<int> arce[],int dfn[],int low[],stack<pair<int,int>> &Stiva,vector <vector <int>> &Comp)
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

    void Vizita(int n,bool vizitat[],vector <int> arce[],stack <int> &L) // DFS-ul grafului ( Vizitat false -> true )
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

    void Asignare(int n,stack <int> &L,bool vizitat[],vector <int> arce_transpuse[],int &nr_comp,vector<int> Comp[]) // DFS-ul grafului transpus ( Vizitat true -> false )
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

    // Pct Critice

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

    void DF3(int n,bool vizitat[],int dfn[],int low[],vector <int> arce[],vector<pair<int,int>> &muchii_critice)
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
                if (low[nr_vcn] > dfn[n]) // Muchie critica
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

    // Sortare topologica

    void SrtTop()
    {
        ifstream fin("sortaret.in");
        ofstream fout("sortaret.out");

        fin>>nr_varfuri>>nr_arce;

        int a,b;
        vector <int> arce[nr_varfuri+2];
        int gradi[nr_varfuri+2]; // Grad intern

        for (int i=1;i<=nr_varfuri;i++)
        {
            gradi[i] = 0;
        }

        for (int i=0;i<nr_arce;i++)
        {
            fin>>a>>b;
            gradi[b]++;
            arce[a].push_back(b);
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

        while (!Coada.empty())
        {
            int nr_crt = Coada.front();
            fout<<nr_crt<<' ';

            int size_nr = arce[nr_crt].size();
            for (int i=0;i<size_nr;i++)
            {
                int vcn_crt = arce[nr_crt][i];
                gradi[vcn_crt]--;
                if (gradi[vcn_crt] == 0)
                    Coada.push(vcn_crt);
            }

            Coada.pop();
        }

        fin.close();
        fout.close();
    }

};

Graf *Graf::graf = nullptr;

Graf *Graf::GetGraf()
{
    if (graf == nullptr)
        graf = new Graf();

    return graf;
}

int main()
{
    Graf::GetGraf()->SrtTop();
    return 0;
}
