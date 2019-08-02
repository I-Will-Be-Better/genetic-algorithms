#include <stdio.h>  
#include <conio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <iostream>  
#include <math.h>

using namespace std;

/*****��ʼ��һЩ����*****/
const int Population_size = 100;        //��Ⱥ��ģ
const int Chromosome_length = 6;    //������Ϊ64����6λ��ʾÿһ������
double rate_crossover = 0.5;                //������
double rate_mutation = 0.001;           //������
int iteration_num = 50;                     //��ʼ�趨����50��                                     
/****************************************/

//��Ⱦɫ�嶨��Ϊ�ṹ������
typedef struct Chromosome                          
{   
    short int bit[Chromosome_length];           //Ⱦɫ��������봮
    double value;                                           //�����ƴ����Ӧ��ʵ��ֵ
    double fitness;                                     //��Ӧֵ  
    double rate_fit;                                        //��Ե�fitֵ������ռ�İٷֱ�
    double cumu_fit;                                    //���۸���  
}chromosome;


/*****��������*****/
//��ʼ���õ�����Ķ������ַ���
void population_initialize(chromosome (&population_current)[Population_size]);
//��Ⱦɫ����н���
void decode(chromosome &population_current) ;   
//����Ⱦɫ�����Ӧ��ֵ
double objective_function(double x);
//������Ⱥ�ڸ��������ֵ
void fresh_property(chromosome(&population_current)[Population_size]);
//������ת���ֵ�ѡ�����    proportional roulette wheel selection
void seletc_prw(chromosome(&population_current)[Population_size], chromosome(&population_next_generation)[Population_size], chromosome &best_individual);
//�������
void crossover(chromosome (&population_next_generation)[Population_size]);  
//ͻ�����
void mutation(chromosome (&population_next_generation)[Population_size]);
/****************************************/

// ������
void main()                                   
{
    /*****��ʼ���������Ⱥ�͸���*****/
    clock_t start, end;//��ʼ��ʱ,��ȷ����
    /****************************************/


    /*****��ʼ���������Ⱥ�͸���*****/
    chromosome population_current[Population_size];                    //��ǰ��Ⱥ  
    chromosome population_next_generation[Population_size];       //��������һ������Ⱥ                        
    chromosome best_individual;                                                 //��¼��Ӧ�ȵ����ֵ
    chromosome zeros_chromosome;                                                //����һ��ȫΪ0�ĸ��壬����Ⱥ����ĳ�����������
    /****************************************/

    int i = 0,j = 0;//ѭ������

    //*****��ʼ���������Ⱥ�͸���*****
    //���ȳ�ʼ��zeros_chromosome����ʹ��֮��ʼ����������
    for (i = 0; i < Chromosome_length; i++)
        zeros_chromosome.bit[i] = 0;
    zeros_chromosome.fitness = 0.0;
    zeros_chromosome.value = 0.0;
    zeros_chromosome.rate_fit = 0.0;
    zeros_chromosome.cumu_fit = 0.0;

    best_individual = zeros_chromosome;
    for (i = 0; i < Population_size; i++)
    {
        population_current[i] = zeros_chromosome;
        population_next_generation[i] = zeros_chromosome;
    }
    /****************************************/

    printf("�Ŵ��㷨���� y = -(x-1)^2 + 5 �����ֵӦ������\n");

enter:printf("��������Ҫ�趨�ĵ������� : ");
    // ����������������͸����� iteration_num
    scanf("%d", &iteration_num);                           

    // �ж�����ĵ��������Ƿ�Ϊ�����㣬�ǵĻ���������
    start = clock();
    if (iteration_num <1)
        goto enter;


    //��Ⱥ��ʼ�����õ�����Ķ������ַ���
    population_initialize(population_current); 
    //������Ⱥ�ڸ��������ֵ
    fresh_property(population_current);
	best_individual = population_current[0];
    // ��ʼ����
    for (i = 0; i< iteration_num; i++)                            
    {
        // �����ǰ��������
        //printf("\ni = %d\n", i); 
        //��ѡ�����������µ���Ⱥ
        seletc_prw(population_current,population_next_generation,best_individual);                 
        //��ѡ������Ⱥ���н������
        crossover(population_next_generation);              
        //�Խ�������Ⱥ���б������
        mutation(population_next_generation);                      
        //������Ⱥ�ڸ��������ֵ
        fresh_property(population_next_generation);
        //��population_next_generation��ֵ����population_current�������population_next_generation��ֵ
        for (j = 0; j < Population_size; j++)
        {
            population_current[j] = population_next_generation[j];
            population_next_generation[j] = zeros_chromosome;
        }
        //����ʱ���Ƿ�90s
        end = clock();
        if (double(end - start) / CLK_TCK> 89)
            break;
    } 
    //�������ʱ��
    printf("\n ����%d������ʱ��Ϊ�� %f\n", iteration_num, double(end - start) / CLK_TCK);

    //������
    printf("\n �����õ����ֵΪ�� %f ,��Ӧ�� x Ϊ��%f \n", best_individual.fitness, best_individual.value);

    for (i = 0; i<Population_size; i++)
    {
        printf(" population_current[%d]=", i);
        for (j = 0; j < Chromosome_length; j++)
            printf(" %d", population_current[i].bit[j]);
        printf("        value=%f    fitness = %f\n", population_current[i].value, population_current[i].fitness);
    }
    printf("\nPress any key to end ! ");


    // ������л�����
//  flushall();                                   
    system("pause");
}


//��������Ⱥ��ʼ��  
//���������������
//����ʱ��ֻ������������
void population_initialize(chromosome (&population_current)[Population_size])   
{
    int i = 0, j = 0;

    //�������������
    srand((unsigned)time(NULL));
    //������Ⱥ�е�ÿ��Ⱦɫ��
    for (j = 0; j<Population_size; j++)                              
    {
        //�����ʼ��Ⱦɫ���ÿһλ
        for (i = 0; i<Chromosome_length; i++)                       
        {
            // �������Ⱦɫ����ÿһ������λ��ֵ��0��1
            population_current[j].bit[i] = rand()% 2;         
        }

    }

}


// �������������ƻ���Ϊʮ���� 
void decode(chromosome &population_current)   
{//�˴���Ⱦɫ�峤��Ϊ6�����е�6����ʾ����λ  
    int i = 0;
    population_current.value = 0;
    //��λ��ǰ����λ�ٺ�
    for( i = Chromosome_length -1 ; i > 0; i-- ) 
        population_current.value += (double)pow(2,Chromosome_length-i-1) * (double)population_current.bit[i];    //����Ⱦɫ������Ʊ���, 
    //���λΪ����λ�������1������
    if (population_current.bit[0] == 1)
        population_current.value = 0 - population_current.value;

}

//����:������Ӧ��
double objective_function(double x)
{
    double y;
    // Ŀ�꺯����y= - ( (x-1)^ 2 ) +5
    y = -((x - 1) *(x - 1)) + 5;                                
    return(y);
}

//������������Ⱥ�ڸ��������ֵ
//˵��������Ⱥ�и���Ķ����ƴ�ȷ���󣬾Ϳ��Լ���ÿ������fitness��value��rate_fit ��cumu_fit
//���룺
//chromosome (&population_current)[Population_size] ��ǰ����Ⱥ������
void fresh_property(chromosome (&population_current)[Population_size])
{
    int j = 0;
    double sum = 0;

    for (j = 0; j < Population_size; j++)
    {

    //Ⱦɫ����룬�������ƻ���Ϊʮ���ƣ��õ�һ������ֵ
        //��������ƴ���Ӧ��10������ֵ
        decode(population_current[j]);                 
        //����Ⱦɫ�����Ӧ��
        population_current[j].fitness = objective_function(population_current[j].value); 
        sum = sum + population_current[j].fitness;

    }


    //����ÿ��Ⱦɫ�����Ӧֵ�ٷֱȼ��ۼ���Ӧ��ֵ�İٷֱȣ������̶�ѡ��ʱ����ѡ��Ⱦɫ��  
    population_current[0].rate_fit = population_current[0].fitness / sum;
    population_current[0].cumu_fit = population_current[0].rate_fit;
    for (j = 1; j < Population_size; j++)
    {
        population_current[j].rate_fit = population_current[j].fitness / sum;
        population_current[j].cumu_fit = population_current[j].rate_fit + population_current[j-1].cumu_fit;
    }


}

//�������������̶�ѡ�񷽷�������Ⱥ�е�Ⱦɫ�����ѡ��  
//���룺
//chromosome (&population_current)[Population_size] ��ǰ����Ⱥ������
//chromosome (&population_next_generation)[Population_size] ѡ�������һ����Ⱥ������
//chromosome &best_individual ��ǰ����Ⱥ�е����Ÿ���
void seletc_prw(chromosome (&population_current)[Population_size],chromosome (&population_next_generation)[Population_size],chromosome &best_individual)
{

    int i = 0, j = 0;
    double rate_rand = 0.0;
    //�������������
    srand((unsigned)time(NULL));
    for (i = 0; i < Population_size; i++)
    {
        rate_rand = (float)rand() / (RAND_MAX);
        if (rate_rand < population_current[0].cumu_fit)
            population_next_generation[i] = population_current[0];      
        else
        {
            for (j = 0; j < Population_size; j++)
            {
                if (population_current[j].cumu_fit <= rate_rand && rate_rand < population_current[j + 1].cumu_fit)
                {
                    population_next_generation[i] = population_current[j + 1];
                    break;
                }
            }
        }

        //�����ǰ�����Ŀǰ�����и��廹Ҫ���㣬�򽫵�ǰ������Ϊ���Ÿ���
        if(population_current[i].fitness > best_individual.fitness)
            best_individual = population_current[i];
    }

}


// �������������
void crossover(chromosome (&population_next_generation)[Population_size])          
{   
    int i = 0,j = 0;
    double rate_rand = 0.0;
    short int bit_temp = 0;
    int num1_rand = 0, num2_rand = 0, position_rand = 0;
    //�������������
    srand((unsigned)time(NULL));
    //Ӧ�����������ٴ��أ����趨Ϊ��Ⱥ����
    for (j = 0; j<Population_size; j++)
    {
        rate_rand = (float)rand()/(RAND_MAX);
        //������ڽ�����ʾͽ��н������
        if(rate_rand <= rate_crossover)
        {
            //�����������Ⱦɫ��
            num1_rand = (int)rand()%(Population_size);
            num2_rand = (int)rand()%(Population_size);
            //�����������Ⱦɫ��Ľ���λ��
            position_rand = (int)rand()%(Chromosome_length - 1);
            //���õ��㽻�棬�����֮���λ������
            for (i = position_rand; i<Chromosome_length; i++)
            {
                bit_temp = population_next_generation[num1_rand].bit[i];
                population_next_generation[num1_rand].bit[i] = population_next_generation[num2_rand].bit[i];     
                population_next_generation[num2_rand].bit[i] = bit_temp;     
            }

        }
    }

}

// �������������
void mutation(chromosome (&population_next_generation)[Population_size])               
{
    int position_rand = 0;
    int i = 0;
    double rate_rand = 0.0;
    //�������������
    srand((unsigned)time(NULL));
    //��������趨Ϊ��Ⱥ����
    for (i = 0; i<Population_size; i++)
    {
        rate_rand = (float)rand()/(RAND_MAX);
        //������ڽ�����ʾͽ��б������
        if(rate_rand <= rate_mutation)
        {
            //�������ͻ��λ��
            position_rand = (int)rand()%(Chromosome_length);
            //ͻ��
            if (population_next_generation[i].bit[position_rand] == 0)
                population_next_generation[i].bit[position_rand] = 1;
            else
                population_next_generation[i].bit[position_rand] = 0;

        }

    }
}
