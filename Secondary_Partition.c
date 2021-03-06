#include"Header1.h"
void Secondary_Partition(FILE* fp1,Net* net_list,Block* bk_list,Tier* tr_list,int N,int B,int T)
{
    int i,j,k;
    int** Cost=(int**)calloc(B,sizeof(int*));
    printf("\n Cost Declared");
    for(i=0;i<B;i++)
    {
        bk_list[i].Current_Cost=calculate_block_cost(bk_list,net_list,i,bk_list[i].tier);
    }
    printf("\n Current Costs calculated");
    for(j=0;j<B;j++)
    {
        Cost[j]=(int*)calloc(T,sizeof(int));
    }
    printf("\n All Cost rows Declared");
    for(i=0;i<B;i++)
    {
        for(j=0;j<T;j++)
        {
            Cost[i][j]=calculate_block_cost(bk_list,net_list,i,j);
        }
    }
    printf("\n Cost calculated");
    Gain* gain_list=(Gain*)calloc(T*B,sizeof(Gain));
    //create_and_link_gain_list(gain_list,bk_list,T,B);
    calculate_gain_list(Cost,gain_list,bk_list,B,T);
    /*FILE* fp=fopen("test_Gain_list_ami49.txt","w");
    k=0;
    for(i=0;i<B;i++)
    {
        fprintf(fp,"\nFor Block %d :",i);
        for(j=0;j<T;j++)
        {
            fprintf(fp,"\n\t\tTier=%d\tGain=%d",j,gain_list[k].gain_value);
            k++;
        }
    }
    fclose(fp);*/
    Compromized_FM(fp1,Cost,gain_list,bk_list,net_list,tr_list,B,N,T);
    printf("\n Gain_list going to be freed");
    free(gain_list);
    for(i=0;i<B;i++)
    {
        free(Cost[i]);
    }
    free(Cost);
}
