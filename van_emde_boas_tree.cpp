#include<bits/stdc++.h>
using namespace std;
#define ll long long int
class Van_emde_boas{
public:
        ll usize,minimum,maximum;  
        Van_emde_boas* summary;
        vector<Van_emde_boas*> clusters;
        
        ll high(ll x){  ll rootu = sqrt(usize);   return (x/rootu);}
        
        ll low(ll x){  ll rootu = sqrt(usize);   return x%rootu;}
        
        ll index(ll i,ll j){ ll rootu = sqrt(usize);  return i*rootu + j;}

        ll VEB_Tree_Minimum(){  return minimum;}

        ll VEB_Tree_Maximum(){  return maximum;}

        Van_emde_boas(ll usize){
            this->usize = usize;
            minimum = -1,maximum = -1;
            if(usize <= 2){
                summary = nullptr;
                clusters = vector<Van_emde_boas*> (0,nullptr);   // just store min and max element
            }

            else{
                ll ncluster = sqrt(usize);
                summary = new Van_emde_boas(ncluster);
                clusters = vector<Van_emde_boas*>(ncluster,nullptr);

                for(ll i = 0; i<ncluster; i++){
                    clusters[i] = new Van_emde_boas(sqrt(usize));
                }
            }
        }

        bool VEB_Tree_Member(ll x){
            if(x == minimum || x == maximum)
                return true;
            else{
                if(usize == 2)  return false;       // because our if is not exe usize = 2 shoud be false
                else{
                    return clusters[high(x)] -> VEB_Tree_Member(low(x));
                }
            }
        }

        void VEB_Empty_Tree_Insert(ll x){
            minimum = x;maximum = x;
        }

        void VEB_Tree_Insert(ll x){
            if(minimum == -1){
                VEB_Empty_Tree_Insert(x);
            }
            else{
                if(x < minimum)
                    swap(minimum,x);

                if(usize > 2){
                    
                    if((clusters[high(x)] -> VEB_Tree_Minimum()) == -1){            // only one recursive call in if and else
                        summary ->VEB_Tree_Insert(high(x));
                        clusters[high(x)] -> VEB_Empty_Tree_Insert(low(x));
                    }
                    else{
                        clusters[high(x)] ->VEB_Tree_Insert(low(x));
                    }

                }

                if(x > maximum)
                    maximum = x;
            }
        }

        ll VEB_Tree_Succesor(ll x){
            if(usize == 2){             // base case
                if(x == 0 && maximum == 1){
                    return 1;
                }
                else{
                    return -1;
                }
            }

            else if(minimum != -1 && x < minimum){
                return minimum;
            }

            else{
                ll max_low = clusters[high(x)] -> VEB_Tree_Maximum();
                if(max_low != -1 && low(x) < max_low){      // we have succesor in our cluster so we don't need to look in the summary 
                    ll offset = clusters[high(x)] -> VEB_Tree_Succesor(low(x));
                    return index(high(x),offset);
                }
                else{                           // looking in summary since we dont have succ in our cluster
                    ll succ_cluster = summary -> VEB_Tree_Succesor(high(x));
                    if(succ_cluster == -1){return -1;}
                    else{
                        ll offset = clusters[succ_cluster] ->VEB_Tree_Minimum();
                        return index(succ_cluster,offset);
                    }

                }
            }
        }

        ll VEB_Tree_Predecessor(ll x)
        {
            if(usize == 2){
                if(x == 1 && minimum == 0){return 0;}
                else{return -1;}
            }
            else if(maximum != -1 && x > maximum){
                return maximum;
            }

            else{
                ll min_low = clusters[high(x)] -> VEB_Tree_Minimum();
                if(min_low != -1 && low(x) > min_low){
                    ll offset = clusters[high(x)]->VEB_Tree_Predecessor(low(x));
                    return index(high(x),offset);
                }
                else{
                    ll pred_cluster = summary ->VEB_Tree_Predecessor(high(x));
                    if(pred_cluster == -1){
                        if(minimum != -1 && x > minimum){       // not storing min
                            return minimum;
                        }
                        else{return -1;}
                    }
                    else{
                        ll offset = clusters[pred_cluster]->VEB_Tree_Maximum();
                        return index(pred_cluster,offset);
                    } 
                }
            }
        }


        void VEB_Tree_Delete(ll x){
            bool include = VEB_Tree_Member(x);          // checking x is part of veb or not
            if(include == false)
                return;
            if(maximum == minimum){     // for only one element
                maximum = -1;minimum = -1;
            }
            else if(usize == 2){    // we have two value and size 2
                    if(x == 0)
                        minimum =1;
                    else
                        minimum = 0;
                    
                    maximum = minimum;
            }
            else{
                if(x == minimum){                   // changing our min value
                    ll firstcluster = summary->VEB_Tree_Minimum();      
                    x = index(firstcluster , clusters[firstcluster]->VEB_Tree_Minimum());
                    minimum = x; 
                }
                clusters[high(x)]->VEB_Tree_Delete(low(x));         // deleting the elemnt  in cluster
                // here maybe cluster empty so update summary

                if(clusters[high(x)]->VEB_Tree_Minimum() == -1){
                    summary->VEB_Tree_Delete(high(x));

                    if(x == maximum){
                        ll summary_max = summary->VEB_Tree_Maximum();
                        if(summary_max == -1){ maximum = minimum;}
                        else{maximum = index(summary_max,clusters[summary_max]->VEB_Tree_Maximum());}
                    }

                }

                else if(x == maximum){
                        maximum = index(high(x),clusters[high(x)]->VEB_Tree_Maximum());
                }
                
            }
        }

};

int main(){
    //freopen("output.txt","w",stdout);
    Van_emde_boas* veb = new Van_emde_boas(16);
    veb->VEB_Tree_Insert(0);
    veb->VEB_Tree_Insert(1);
    veb->VEB_Tree_Insert(2);
    veb->VEB_Tree_Insert(5);
    veb->VEB_Tree_Insert(8);
    veb->VEB_Tree_Insert(11);
    veb->VEB_Tree_Insert(13);
    veb->VEB_Tree_Insert(14);
    veb->VEB_Tree_Insert(15);

    cout<<veb->maximum<<"\n";
    cout<<veb->minimum<<"\n";
    cout<<veb->VEB_Tree_Member(1)<<"\n";
    cout<<veb->VEB_Tree_Succesor(-1)<<"\n";
    cout<<veb->VEB_Tree_Succesor(2)<<"\n";
    veb->VEB_Tree_Delete(0);
    veb->VEB_Tree_Delete(5);
    cout<<veb->minimum<<"\n";
    cout<<veb->VEB_Tree_Succesor(2)<<"\n";
    cout<<veb->VEB_Tree_Predecessor(1)<<"\n";

}
