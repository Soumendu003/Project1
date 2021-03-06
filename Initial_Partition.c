#include"Header1.h"
void Initial_Partition(FILE* fp1,Block* bk_list,Net* net_list,int B,int N,int T,float relax)
{
    int i,cnt,flag,tier_cnt;
    double avg_area,tot_area=Calculate_Total_Area(bk_list,B);
    avg_area=tot_area/T;
    fprintf(fp1,"\n***********************************************************************************************");
    fprintf(fp1,"\n");
    fprintf(fp1,"\n***********************************************************************************************");
    fprintf(fp1,"\n No of Tiers=%d\t Relaxation Percentage=%f",T,relax);
    fprintf(fp1,"\n Avg area for each tier=%lf",avg_area);
    fprintf(fp1,"\n Max area for each tier=%lf",avg_area*(1+relax));
    fprintf(fp1,"\n Min area for each tier=%lf",avg_area*(1-relax));
    Tier* tier_list=(Tier*)calloc(T,sizeof(Tier));
    //Initializes Tier Components
    for(i=0;i<T;i++)
    {
        tier_list[i].area_consumed=0;
        tier_list[i].max_area=avg_area*(1+relax);
        tier_list[i].min_area=avg_area*(1-relax);
        tier_list[i].tot_bk=0;
    }
    initialize_net_list(net_list,N);
    default_blocks_placement(bk_list,B);
    cnt=0;
    tier_cnt=0;
    flag=0;
    clock_t start_time=clock();
    for(i=0;i<N;i++)
    {
        Block_Component* tem=net_list[i].bk_ptr;
        while(tem!=NULL)
        {
            int bk_index=tem->bk_index;
            if(bk_list[bk_index].tier==-1)
            {
                int lock=0;
                while(!place_block(tier_list,bk_list,bk_index,tier_cnt,bk_list[bk_index].tier))   //place_blocks returns 1 if it successfully places the bk at tier_cnt
                {
                    lock++;
                    tier_cnt++;
                    tier_cnt=(tier_cnt%T);
                    if(lock>=T)
                    {
                        printf("\n The Block Index=%d is locked",bk_index);
                        flag=1;
                        break;
                    }
                }
                tier_cnt++;
                tier_cnt=(tier_cnt%T);
                cnt++;
                if(cnt>=B)
                {
                    flag=1;               //indicates that all the blocks are placed
                    break;
                }
            }
            tem=tem->right;
        }
        if(flag!=0)
        {
            break;
        }
    }
    clock_t end_time=clock();
    printf("\n Initial Partitioning Done");
    custom_update_net_list(net_list,bk_list,N,B,T);
    /*FILE* fp=fopen("Net_list_after_initial_partition_ami49.txt","w+");
    for(i=0;i<N;i++)
    {
        fprintf(fp,"\nThe Net Degree=%d\t Number of Blocks=%d",net_list[i].degree,net_list[i].no_of_bk);
        fprintf(fp,"\nGND pin=%d\tPOW pin=%d",net_list[i].gnd,net_list[i].pwr);
        fprintf(fp,"\nTop_tier=%d\t Low_Tier=%d",net_list[i].top_tier.tier_index,net_list[i].low_tier.tier_index);
    }
    fclose(fp);*/
  /*  fp=fopen("Block_initial_placement_ami49.txt","w+");*/
    double time_taken=(double)(end_time-start_time)/CLOCKS_PER_SEC;
    fprintf(fp1,"\nTime Taken to execute Initial Partition:%0.6lf",time_taken);
    fprintf(fp1,"\nAfter Initial Partition Total Number of MIV=%d",claculate_MIV(bk_list,net_list,N));
    /*for(i=0;i<B;i++)
    {
        fprintf(fp,"\n Block_Name=%s\t Block_Index=%d\t Block_Tier=%d",bk_list[i].name,bk_list[i].index,bk_list[i].tier);
    }
    fclose(fp);*/
    printf("\n Calling Secondary Partition:");
    Secondary_Partition(fp1,net_list,bk_list,tier_list,N,B,T);
    free(tier_list);
    printf("\n Tier list freed");

}
