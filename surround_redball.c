
/*

 surround_redball.c



 未完成です

 赤玉の動きが思った通りになりません
 何回かル-プするたびにいきなり思いもよらぬ端に飛んでいくことがあります

 間に合いませんでした

 

  基礎プロ2最終課題
  853554
  Kazuhito Tokita


 --- ゲ-ム説明 ---
  
  外側に逃げようとする赤い球を黒い球で囲むゲ-ムです
  赤玉が端に行ったらゲ-ムオ-バ-




-----------------------------------------------------------------------------------------------


  赤玉の最短距離の計算方法の流れ

 1 円にIDを振り当てる
 2 隣接行列を作る
 3 幅優先探索により探索する
 4 3での結果をもとにそれぞれ道を一つに繋げる
 5 4の中で一番距離が短いものを選ぶ(複数ある場合はランダムで選ぶ)


 特に4番がかなり詰まりました
 幅優先による結果を見て規則性を探し,それを実行することがかなり困難で様々な工夫を凝らしました


-----------------------------------------------------------------------------------------------



  タ-ミナル上で確認したりする作業のprintfなどはコメントとしてあえて残しています


                                                                                                 */

#include<stdio.h>
#include<stdlib.h>
#include<handy.h>
#include<time.h>

#define WINDOWSIZE 500
#define RADIUS 15.0
#define WIDTH 5//円と円の間隔
#define VERTICAL 18.02775637731995
#define N 121


int v[N]={};//訪問フラグ
int adjacency[N][N]={};//隣接行列
int circle[11][11]={};


void field(int x1,int x2,int y1,int y2,
           int x1start,int x2start,int y1start,int y2start,
           int x1plus,int x2plus,int y1plus,int y2plus);
void adjacencymatrix();//隣接行列初期値設定
void oddblackstop(int i,int j);//奇数行にある黒玉の隣接行列の設定
void evenblackstop(int i,int j);//偶数行にある黒玉の隣接行列の設定



int main() {
    

    HgOpen(WINDOWSIZE,WINDOWSIZE);
    
    int size=40;
    HgSetFillColor(HG_BLACK); //文字の色を黒色にする
    HgSetFont(HG_GB,size); //文字のフォントと大きさ
    HgText(120,280, "赤玉を囲め!",size);
    HgText(80,100,"START");
    HgSetFont(HG_GB,size/2); //文字のフォントと大きさ
    HgText(150,70,"(キ-を押してください)");

    hgevent *event;              //
    HgSetEventMask(HG_KEY_DOWN); //キ-を押したら
    HgGetEventMask();            //次のプログラムへと実行される
    event=HgEvent();             //

    HgClear();
    
    int x1,y1;//奇数行目の円の座標
    int x2,y2;//偶数行目の円の座標
    int i,j,k,l,m,n;//カウンタ変数
    int vnum=0;
    
    double y1start,y2start,x1start,x2start;
    double y1plus,y2plus,x1plus,x2plus;
    
    //x1,x2,y1,y2の初期値
    y1start=RADIUS+50;
    y2start=RADIUS+50+(WIDTH*2+RADIUS*4)/2;
    x1start=RADIUS+50;
    x2start=RADIUS+50+(WIDTH+RADIUS*2)/2;
    
    //x1,x2,y1,y2の中心から中心の距離
    y1plus=WIDTH*2+RADIUS*4;
    y2plus=WIDTH*2+RADIUS*4;
    x1plus=WIDTH+RADIUS*2;
    x2plus=WIDTH+RADIUS*2;
    

  
    /*                                フィ-ルド                                        */
    
    field(x1,x2,y1,y2,x1start,x2start,y1start,y2start,x1plus,x2plus,y1plus,y2plus);

    //circle[i][j]に0~120までの番号振り当て(IDを振り当てる)
    int num=0;
    for(i=0; i<11; i++){
        for(j=0; j<11; j++){
            circle[i][j]=num;
            num++;
        }
    }

    
    /*                隣接行列初期値設定(隣り合う場合,つまり移動可能な円を1とする)                 */
    
    
    adjacencymatrix();
    
    
             
    
    /*                                      ランダムに黒玉を配置                                    */

    
    srand(time(NULL));

    
    int rand1,rand2;
    int random[11]={0,1,2,3,4,5,6,7,8,9,10};//横一列0~10番目までの配列
    int oddI[6+1]={0,0,2,4,6,8,10};//1番目から数えるため0番目はなんでも良い,0にした
    int evenI[5+1]={0,1,3,5,7,9};//1番目から数えるため0番目はなんでも良い,0にした
    int count1=0;//カウンタ変数
    int count2=0;//カウンタ変数
    int oddblacknum[5]={2,3,4,5,6};//奇数行の黒玉の数
    int evenblacknum[5]={3,4,5,6,7};//偶数行の黒玉の数
    int oddrand;
    int evenrand;
    int befoddrand[6]={};//既出の奇数行
    int befevenrand[5]={};//既出の偶数行
    int count0;//ダブり判別のカウンタ変数

    int oddblackx[N]={};//奇数行の黒玉のx座標を格納
    int oddblacky[N]={};//奇数行の黒玉のy座標を格納
    int evenblackx[N]={};//偶数行の黒玉のx座標を格納
    int evenblacky[N]={};//偶数行の黒玉のy座標を格納
    int oddblackcount=0;//カウンタ変数
    int evenblackcount=0;//カウンタ変数
    


    /*                                奇数行                                     */
    oddrand=rand()%5;

    k=0;


    /*             ダブりを防ぐコ-ド         */
    
          /*        黒玉の数         */
    while(count1<oddblacknum[oddrand]){
        
        rand1=rand()%6+1;//0をなくすため+1
        rand2=rand()%11;
        /*  ダブりを防ぐためのコ-ド_既出でないrand1が出るまでル-プさせる */
        /*  各行に2つ以上の黒玉は出さない                                */
        for(;;){
            count0=0;
            for(l=0; l<k; l++){
                if(rand1==befoddrand[l]){
                    rand1=rand()%6+1;
                }
                else{
                    count0++;
                }
            }
            if(count0==k){//ダブりがないならば
                break;
            }
        }


        /*               黒玉を出力するコ-ド               */
        for(y1=y1start,i=0; i<11; y1=y1+y1plus){
            for(x1=x1start,j=0; j<11; x1=x1+x1plus){
                if((i==oddI[rand1])&&(j==random[rand2])){

                    befoddrand[k]=rand1;//ダブりをなくすため既出の行を格納する
                    k++;
                    
                    HgSetFillColor(HG_BLACK);
                    HgCircleFill(x1,y1,RADIUS,1);

                    oddblackx[oddblackcount]=x1;//黒玉のx座標を格納
                    oddblacky[oddblackcount]=y1;//黒玉のy座標を格納
                    oddblackcount++;


                    oddblackstop(i,j);
                    
                  }
                  
                  j++;
            }
            i=i+2;
        }
        count1++;
    }


    /*                                  偶数行                                   */
    evenrand=rand()%5;
    
    k=0;


     /*             ダブりを防ぐコ-ド         */

          /*        黒玉の数         */
    while(count2<evenblacknum[evenrand]){
        
        rand1=rand()%5+1;//0をなくすため+1
        rand2=rand()%11;

        /*  ダブりをなくすためのコ-ド_既出でないrand2が出るまでル-プさせる  */
        /*  各行には2つ以上の黒玉は出さない                                 */
        for(;;){
            count0=0;
            for(l=0; l<k; l++){
                if(rand2==befevenrand[l]){
                    rand2=rand()%5+1;
                }
                else{
                    count0++;
                }
            }
                if(count0==k){
                    break;
                }
            }


         /*               黒玉を出力するコ-ド               */
        for(y2=y2start,i=1; i<11; y2=y2+y2plus){
            for(x2=x2start,j=0; j<11; x2=x2+x2plus){
                   if((i==evenI[rand1])&&(j==random[rand2])){

                       befevenrand[k]=rand2;//ダブりをなくすため既出の行を格納する
                       k++;
                       
                       HgSetFillColor(HG_BLACK);
                       HgCircleFill(x2,y2,RADIUS,1);

                       evenblackx[evenblackcount]=x2;//黒玉のx座標を格納
                       evenblacky[evenblackcount]=y2;//黒玉のy座標を格納
                       evenblackcount++;
                       
                       evenblackstop(i,j);
                       
                   }
                   j++;
            }
            i=i+2;
        }
        count2++;
    }

    
    
    /*          敵を真ん中に配置         */

    int x0,y0;//現在敵がいる座標

    x0=x2start+x2plus*5;
    y0=y2start+y2plus*2;
    
    HgSetFillColor(HG_RED);
    HgCircleFill(x0,y0,RADIUS,1);



    int currentcircle;//現在いる円のID番号

    currentcircle=60;//敵の初期値はID60番目





    /*                             "リックする⇄敵が動く" の繰り返し                             */
    int flag;
    double x,y;//クリック時の座標

    HgSetEventMask(HG_MOUSE_DOWN);//マウスクリックのみを検出するように設定


  
    for(;;){
        
/*                          クリックした円を黒くする                         */ 
        
        for(;;){

            flag=0;
            
            
            event=HgEvent();//マウスクリックを待ってクリックされた情報はeventに格納
            x=event->x;
            y=event->y;
            
            
            
            for(y1=y1start,i=0; i<11; y1=y1+y1plus){
                for(x1=x1start,j=0; j<11; x1=x1+x1plus){
                    
                    if(((x1-x)*(x1-x)+(y1-y)*(y1-y))<=RADIUS*RADIUS){
                        flag=1;
                        HgSetFillColor(HG_BLACK);
                        HgCircleFill(x1,y1,RADIUS,1);

                        /*  黒玉をクリックしたらもう一度クリックできる */
                        for(k=0; k<oddblackcount; k++){
                            if(((oddblackx[k]-x)*(oddblackx[k]-x)+
                                (oddblacky[k]-y)*(oddblacky[k]-y))
                               <=RADIUS*RADIUS){
                                flag=0;
                            }
                        }

                        /* 赤玉をクリックしたらもう一度クリックできる    */
                        if(((x0-x)*(x0-x)+(y0-y)*(y0-y))<=RADIUS*RADIUS){
                            HgSetFillColor(HG_RED);
                            HgCircleFill(x0,y0,RADIUS,1);
                            flag=0;
                        }
                        
                            

                        oddblackx[oddblackcount]=x1;
                        oddblacky[oddblackcount]=y1;
                        oddblackcount++;
                        
                
                        oddblackstop(i,j);//奇数行の隣接行列
                    }
                    j++;
                }
                i=i+2;
            }
            
            for(y2=y2start,i=1; i<11; y2=y2+y2plus){
                for(x2=x2start,j=0; j<11; x2=x2+x2plus){
                    
                    if(((x2-x)*(x2-x)+(y2-y)*(y2-y))<=RADIUS*RADIUS){
                        flag=1;
                        HgSetFillColor(HG_BLACK);
                        HgCircleFill(x2,y2,RADIUS,1);

                        /* 黒玉をクリックしたらもう一度クリックできる */
                        for(k=0; k<evenblackcount; k++){
                            if(((evenblackx[k]-x)*(evenblackx[k]-x)+
                                (evenblacky[k]-y)*(evenblacky[k]-y))
                               <=RADIUS*RADIUS){
                                flag=0;
                            }
                        }

                        /* 赤玉をクリックしたらもう一度クリックできる   */
                        if(((x0-x)*(x0-x)+(y0-y)*(y0-y))<=RADIUS*RADIUS){
                            HgSetFillColor(HG_RED);
                            HgCircleFill(x0,y0,RADIUS,1);
                            flag=0;
                        }
                        


                        evenblackx[evenblackcount]=x2;
                        evenblacky[evenblackcount]=y2;
                        evenblackcount++;
                        
                        
                        evenblackstop(i,j);//偶数行の隣接行列
                    }
                    j++;
                }
                i=i+2;
            }
                   
                   
            
            
            if(flag==1){
                break;
            }
            
            
            
        }
        
        
    
/*                                     隣接行列を表示                                  */ 
    /*   for(k=0; k<11; k++){
        for(l=0; l<11; l++){
            for(m=0; m<11; m++){
                for(n=0; n<11; n++){

                    printf("%d ",adjacency[circle[k][l]][circle[m][n]]);

                }
                printf("/");
            }

            printf("\n\n");
            printf("%d  %d",k,l);
            printf("\n\n");
        }
    }
    */

    
    /*                                   探索開始                                    */

    ////////////////////////////////////幅優先探索///////////////////////////////////

    int count=0;//num1,num2の要素数
    int head=0;
    int tail=0;
    int queue[500]={};
    int num1[500]={};
    int num2[500]={};
    int visited[500]={};
    k=0;
    l=0;

    flag=0;

    //訪問フラグの初期化 (<- これがないことでかなり詰まった)
    for(j=0; j<N; j++){
        v[j]=0;
    }
    

    queue[tail++]=currentcircle;
    v[currentcircle]=1;

    //一度通った道をvisitedに格納して訪問フラグ1をつける
    visited[vnum]=currentcircle;
    vnum++;
    for(j=0; j<vnum; j++){
        v[visited[vnum]]=1;
    }
    
    
    do{
        i=queue[head++];
        for(j=0; j<N; j++){
            if(adjacency[i][j]==1 && v[j]==0){
                //  printf("%d ->%d ",i,j);//全ての探索経路が表示(繋がってはいない=まばらな状態で)

                num1[k]=i;
                k++;

                num2[l]=j;
                l++;

                count++;
                /*                端っこに行ったら探索終了              */
                if((0<=j && j<=10) || (110<=j && j<=120) ||
                   j==11 || j==22 || j==33 || j==44 || j==55 || j==66 ||
                   j==77 || j==88 || j==99 ||
                   j==21 || j==32 || j==43 || j==54 || j==65 || j==76 ||
                   j==87 || j==98 || j==109){
                    // printf("goal\n");
                    flag=1;
                    break;
                }
                
                queue[tail++]=j;
                v[j]=1;
                
            }
        }
        //  printf("\n");
    } while( head != tail);
    

    /*                         赤玉を囲めた場合-ゲ-ムクリア!                          */
    

    if(flag==0){//"端っこに行ったら探索終了"のif分に一度も入らない場合ゲ-ムクリア
          break;
     }
    
        
        
   

    printf("\n");

    /*    今の状態はタ-ミナル上で見て飛び飛びでゴ-ルまでの道順がわかる   */
    /*    次に各道順を一つに繋げたい    */
    /*    そのためnum1とnum2を見比べて規則性を見つけることにした       */




    
    //num1表示
    /*   printf("num1\n");
    for(k=0; k<count; k++){
        printf("%d ",num1[k]);
    }
    
    printf("\n \n");
    
    //num2表示
    printf("num2\n");
    for(l=0; l<count; l++){
        printf("%d ",num2[l]);
    }
    
    
    printf("\n \n");
    */


    
    
    /*    ゴ-ルから辿ってみるために逆順にする    */ 
    
    //num1を逆順にする
    for(k=0; k<count/2; k++){
        int t=num1[k];
        num1[k]=num1[count-k-1];
        num1[count-k-1]=t;
    }
    
    //num2を逆順にする
    for(l=0; l<count/2; l++){
        int u=num2[l];
        num2[l]=num2[count-l-1];
        num2[count-l-1]=u;
    }


    /*
    //num1逆順に表示
    printf("num1逆順\n");
    for(k=0; k<count; k++){
        printf("%d ",num1[k]);
    }
    printf("\n\n");
    
    //num2逆順に表示
    printf("num2逆順\n");
    for(l=0; l<count; l++){
        printf("%d ",num2[l]);
    }
    printf("\n \n");
    */
    
    /*                  path[m][0]にgoal地点を格納                   */
    
    int path[500][500]={};
    m=0;
    n=0;
    for(l=0; l<count; l++){
        if((0<=num2[l] && num2[l]<=10) || (110<=num2[l] && num2[l]<=120)
           || num2[l]==11 || num2[l]==22 || num2[l]==33 || num2[l]==44
           || num2[l]==55 || num2[l]==66 || num2[l]==77 || num2[l]==88
           || num2[l]==99
           || num2[l]==21 || num2[l]==32 || num2[l]==43 || num2[l]==54
           || num2[l]==65 || num2[l]==76 || num2[l]==87 || num2[l]==98
           || num2[l]==109
            ){
            path[m][n]=num2[l];
            m++;//m通り道順がある
        }
    }
    
    
    int pathnum;//道順の数(何通り道順があるか)
    
    pathnum=m;
    
/*    printf("ゴ-ル地点\n");
    for(m=0; path[m][n]>0; m++){
        printf("%d ",path[m][n]);
    }
*/
    
    
    /* ゴ-ル地点path[m][0]に格納されている値がnum2の配列の何番目に格納されているかを調べる              */
    /* しかし,ゴ-ル地点に同じ数字が含まれる場合上手く何番目にあるか調べられない                      */
    /* そこで工夫した考えが                                                                              */
    /* 数字ごとにnum2で数えて何番目かをanswerという配列に格納して,answerという配列を昇順にすれば        */
    /* ゴ-ル地点の値がnum2で何番目か一致すると考えた                                                    */
    /* しかし,0があるために昇順では上手くいかないので一旦降順にしてから0より大きい値がいくつあるか数える*/
    /* answerという配列をその要素数分だけにして逆順にすると各ゴ-ル地点がnum2で何番目かが判断できる      */
    
    //  printf("\n\n");
    
    int answer[30][5]={};

    //answer[i][0]にゴ-ル地点の値を格納する
    i=0;
    for(m=0; m<pathnum; m++){
        answer[i][0]=path[m][0];
        i++;
    }
    
    //  printf("\n\n");
    
    //ゴ-ル地点に同じ値が2つ以上あればそれがnum2で何番目にあるかをanswer[i][1~j]に格納する
    j=1;
    for(i=0; i<pathnum; i++){
        for(k=0; k<count; k++){
            if(answer[i][0]==num2[k]){
                answer[i][j]=k;
                j++;
            }
        }
        j=1;
    }
    
        int ascending[50]={};//昇順
        
        
        ascending[0]=0;//必ずpath[0][0]はnum2[0]にくるので
        k=1;

        
        //ascendingにanswerの0よりも大きい値を全て格納する   
        for(i=0; i<pathnum; i++){
            for(j=1; j<5; j++){
                if(i==0 && j==1){
                    break;
                }
                if(answer[i][j]>0){
                    ascending[k]=answer[i][j];
                    k++;
                }
            }
        }
        
        
        //同じ値があれば片方を0にする
        for(k=0; k<50; k++){
            for(i=1; i<50; i++){
                if(ascending[k]==ascending[k+i]){
                    ascending[k+i]=0;
                }
            }
        }
        
        //降順にする
        int tmp;
        
        for(i=0; i<50; i++){
            for(j=i+1; j<50; j++){
                if(ascending[i]<ascending[j]){
                    tmp=ascending[i];
                    ascending[i]=ascending[j];
                    ascending[j]=tmp;
                }
            }
        }


        //要素が0になるまでに何個の要素が入っているか調べる
        //0は不必要な数なので数えやすくするために降順にした
        int ascendcount=0;
             
        for(i=0; i<50; i++){
            if(ascending[i]>0){
                ascendcount++;
            }
        }
        
        //0が一つ入るため
        ascendcount=ascendcount+1;
        
        //逆順にする
        for(j=0; j<ascendcount/2; j++){
            int t=ascending[j];
            ascending[j]=ascending[ascendcount-j-1];
            ascending[ascendcount-j-1]=t;
        }

        //ascending[i]を表示
        /*     printf("ascneding\n");
        for(i=0; i<ascendcount; i++){
            printf("%d ",ascending[i]);
        }
        
        printf("\n\n");
        */

        
        /*    ascending[ascendcount]にnum2でゴ-ル地点がそれぞれ何番目にあるかを格納している    */
        /*    path[m][0]とascending[ascendcount]は対応している   */
        
        
        //  printf("\n\n");

        //num1とnum2の比較による規則性は,num1[k]とnum2[l]の値が同じ時のnum1[l]が次の進む値になる
        //これをプログラムで表すと下記のようになる
        //やっとできた!!!!!!!!!!!ここまでががかなり苦労した!
        //幅優先探索による道順の全通りをpath[m][n]に格納
        
        int box[100]={};
        m=0;
        for(i=0; i<ascendcount; i++){
            int o=0;
            n=1;
            for (l=0; l<count; l++){
                if(num1[ascending[i]]==num2[l]){
                    path[m][n]=num2[l];
                    box[o]=num1[l];
                }
                if(box[o]==num2[l]){
                    n++;
                    path[m][n]=num2[l];
                    o++;
                    box[o]=num1[l];
                }
            }
            m++;
        }



     
                
/*
        //path[m][n]を表示
        printf("\n");
        printf("幅優先探索による各道順\n");
        for(m=0; m<pathnum; m++){
            printf("path[%d][n]: ",m);
            for(n=0; path[m][n]>0; n++){
                printf("%d ",path[m][n]);
                
            }
            printf("\n");
        }
*/


        
        //各path[m][n]のnにある0よりも大きい要素数を数える
        int pathcount[30]={};
        
        j=0;
        for(m=0; m<pathnum; m++){
            i=0;
            for(n=0; n<10; n++){
                if(path[m][n]>0){
                    i++;
                }
                else{
                    pathcount[j]=i;
                    j++;
                    break;
                }
            }
        }
        
        
        /*       pathcount[j]での最小値が最短距離path[j][pathcount[j]]になる         */

        int min;

        //最小値を求める
        min=pathcount[0];
        for(j=0; j<pathnum; j++){
            if(min>pathcount[j]){
                min=pathcount[j];
            }
        }
         
        int shotestdis[10][5]={};//pathの中の最短距離だけをshotestdisに入れ替える

        m=0;
        for(j=0; j<pathnum; j++){
            if(min==pathcount[j]){
                i=0;
                for(n=0; n<min; n++){
                    shotestdis[m][n]=path[j][i];
                     i++;
                }
                m++;
            }
        }

        int shotestdisnum;//shotestdisの道順数(shotestdis[m][n]のmの数,mの要素数)
        
        shotestdisnum=m;



        
/*        
        printf("最短距離\n");
        for(m=0; m<shotestdisnum; m++){
            printf("shotestdis[%d][0~%d]:",m,n);
            for(n=0; n<min; n++){
                printf("%d ",shotestdis[m][n]);
            }
            printf("\n");
        }
        
        printf("\n \n");
*/




        

        /*           shotestdisnum個ある最短距離の道順の中からランダムで一つ道を選ぶ          */
        
        int pathrand;

        pathrand=rand()%shotestdisnum;

        
        //ゴ-ル地点から探していたため,また逆順に戻す
        //   printf("\n\n");
        for(n=0; n<min/2; n++){
           int t=shotestdis[pathrand][n];
            shotestdis[pathrand][n]=shotestdis[pathrand][min-n-1];
            shotestdis[pathrand][min-n-1]=t;
        }

        printf("実行する最短距離の道\n");
        printf("shotestdis[%d][0~%d]:",pathrand,min);
        for(n=0; n<min; n++){
            printf("%d ",shotestdis[pathrand][n]);
        }

        printf("\n\n");
        

        //これにより現在いる地点から次に移動する地点はshotestdis[pathrand][0]である

/*                                    handyでの敵の動きを描写する                                    */

        flag=0;
        
        for(y1=y1start,i=0; i<11; y1=y1+y1plus){
            if(flag==1){
                break;//赤玉が端っこに行けばゲ-ム終了(無限ル-プから抜け出す)
            }
            for(x1=x1start,j=0; j<11; x1=x1+x1plus){
                if(shotestdis[pathrand][0]==circle[i][j]){
                    HgSetFillColor(HG_RED);
                    HgCircleFill(x1,y1,RADIUS,1);//移動する円
                    HgSetFillColor(HG_GREEN);
                    HgCircleFill(x0,y0,RADIUS,1);//現在いる円(離れる円)
                    x0=x1;
                    y0=y1;
                    currentcircle=circle[i][j];

                    /*                   端っこに行けば                    */
                    if(((i==0 || i==10)&& (0<=j || j<=10)) ||
                       ((i==2 || i==4 || i==6 || i==8) && (j==0 || j==10))){
                        flag=1;
                    }
                    
                } 
                j++;
            }
            i=i+2;
        }

        if(flag==1){
            break;
        }
        
        for(y2=y2start,i=1; i<11; y2=y2+y2plus){
            if(flag==2){
                break;//赤玉が端っこに行けばゲ-ム終了(無限ル-プから抜け出す)
            }
            for(x2=x2start,j=0; j<11; x2=x2+x2plus){
                if(shotestdis[pathrand][0]==circle[i][j]){
                    HgSetFillColor(HG_RED);
                    HgCircleFill(x2,y2,RADIUS,1);//移動する円
                    HgSetFillColor(HG_GREEN);
                    HgCircleFill(x0,y0,RADIUS,1);//現在いる円(離れる円)
                    x0=x2;
                    y0=y2;
                    currentcircle=circle[i][j];

                    /*                      端っこに行けば                       */
                    if((i==1 || i==3 || i==5 || i==7 || i==9) && (j==0 || j==10)){
                        flag=2;
                    }
                    
                }
                j++;
            }
            i=i+2;
        }

        if(flag==1 || flag==2){
            break;
        }







        


}


 

    
    //geme over
    if(flag==1 || flag==2){
        HgText(120,450,"GAME OVER !");
    }
    




    
    
    //gemaclear
    if(flag==0){
        HgText(120,450,"GAME CLEAR !");
    }
    
        

    //キ-を押したらプログラム終了
    HgSetEventMask(HG_KEY_DOWN); 
    HgGetEventMask();            
    event=HgEvent();

    


        
        
        HgGetChar();
        HgClose();
        
        return 0;
}


















void field(int x1,int x2,int y1,int y2,
           int x1start,int x2start,int y1start,int y2start,
           int x1plus,int x2plus,int y1plus,int y2plus)
{
        /*                    フィ-ルド                    */

    int i,j;

    //下から奇数番目の行
    HgSetFillColor(HG_GREEN);
    
    for(y1=y1start,i=0; i<11; y1=y1+y1plus){
        for(x1=x1start,j=0; j<11; x1=x1+x1plus){
            circle[i][j]=HgCircleFill(x1,y1,RADIUS,1);
            j++;
        }
        i=i+2;
    } 

    //下から偶数番目の行
    for(y2=y2start,i=1; i<11; y2=y2+y2plus){
        for(x2=x2start,j=0; j<11; x2=x2+x2plus){
            circle[i][j]=HgCircleFill(x2,y2,RADIUS,1);
            j++;
        }
        i=i+2;
    }
}


void adjacencymatrix(void)
{
    int k,l;
    
 //隣接行列初期値設定(隣り合う場合、移動可能な円を1とする)
    for(k=0; k<11; k++){
        for(l=0; l<11; l++){

            //偶数行
            if(k%2==1){
                if(k-1>=0){
                    adjacency[circle[k][l]][circle[k-1][l]]=1;
                }
                if((k-1>=0)&&(l+1<=10)){
                    adjacency[circle[k][l]][circle[k-1][l+1]]=1;
                }
                if(l-1>=0){
                    adjacency[circle[k][l]][circle[k][l-1]]=1;
                }
                if(l+1<=10){
                    adjacency[circle[k][l]][circle[k][l+1]]=1;
                }
                if(k+1<=10){
                    adjacency[circle[k][l]][circle[k+1][l]]=1;
                }
                if((k+1<=10)&&(l+1<=10)){
                    adjacency[circle[k][l]][circle[k+1][l+1]]=1;
                }
            }
            
            //奇数行
            if(k%2==0){
                if((k-1>=0)&&(l-1>=0)){
                    adjacency[circle[k][l]][circle[k-1][l-1]]=1;
                }
                if(k-1>=0){
                    adjacency[circle[k][l]][circle[k-1][l]]=1;
                }
                if(l-1>=0){
                    adjacency[circle[k][l]][circle[k][l-1]]=1;
                }
                if(l+1<=10){
                    adjacency[circle[k][l]][circle[k][l+1]]=1;
                }
                if((k+1<=10)&&(l-1>=0)){
                    adjacency[circle[k][l]][circle[k+1][l-1]]=1;
                }
                if(k+1<=10){
                    adjacency[circle[k][l]][circle[k+1][l]]=1;
                }
            }
        }
    }
    
}



void oddblackstop(int i,int j){
    
    //黒玉は通行不可(移動不可は0とする)
    if((i-1>=0)&&(j-1>=0)){
        adjacency[circle[i-1][j-1]][circle[i][j]]=0;
    }
    if(i-1>=0){
        adjacency[circle[i-1][j]][circle[i][j]]=0;
    }
    if(j-1>=0){
        adjacency[circle[i][j-1]][circle[i][j]]=0;
    }
    if(j+1<=10){
        adjacency[circle[i][j+1]][circle[i][j]]=0;
                }
    if((i+1<=10)&&(j-1>=0)){
        adjacency[circle[i+1][j-1]][circle[i][j]]=0;
    }
    if(i+1<=10){
        adjacency[circle[i+1][j]][circle[i][j]]=0;
    }
    
    
    //黒玉からはどこにも行けない(自明)
    if((i-1>=0)&&(j-1>=0)){
        adjacency[circle[i][j]][circle[i-1][j-1]]=0;
    }
    if(i-1>=0){
        adjacency[circle[i][j]][circle[i-1][j]]=0;
    }
    if(j-1>=0){
        adjacency[circle[i][j]][circle[i][j-1]]=0;
    }
    if(j+1<=10){
        adjacency[circle[i][j]][circle[i][j+1]]=0;
    }
    if((i+1<=10)&&(j-1>=0)){
        adjacency[circle[i][j]][circle[i+1][j-1]]=0;
    }
    if(i+1<=10){
        adjacency[circle[i][j]][circle[i+1][j]]=0;
    }
    
    
}



void evenblackstop(int i,int j){
    
    //黒玉は通行不可(移動不可は0とする)
    if(i-1>=0){
        adjacency[circle[i-1][j]][circle[i][j]]=0;
    }
    if((i-1>=0)&&(j+1<=10)){
        adjacency[circle[i-1][j+1]][circle[i][j]]=0;
    }
    if(j-1>=0){
        adjacency[circle[i][j-1]][circle[i][j]]=0;
    }
    if(j+1<=10){
        adjacency[circle[i][j+1]][circle[i][j]]=0;
    }
    if(i+1<=10){
        adjacency[circle[i+1][j]][circle[i][j]]=0;
    }
    if((i+1<=10)&&(j+1<=10)){
        adjacency[circle[i+1][j+1]][circle[i][j]]=0;
    }
    
    
    
    //黒玉からはどこにも行けない(自明)
    if(i-1>=0){
        adjacency[circle[i][j]][circle[i-1][j]]=0;
    }
    if((i-1>=0)&&(j+1<=10)){
        adjacency[circle[i][j]][circle[i-1][j+1]]=0;
    }
    if(j-1>=0){
        adjacency[circle[i][j]][circle[i][j-1]]=0;
    }
    if(j+1<=10){
        adjacency[circle[i][j]][circle[i][j+1]]=0;
    }
    if(i+1<=10){
        adjacency[circle[i][j]][circle[i+1][j]]=0;
    }
    if((i+1<=10)&&(j+1<=10)){
        adjacency[circle[i][j]][circle[i+1][j+1]]=0;
    }
    
    
}


