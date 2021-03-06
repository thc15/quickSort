#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

//#define DEBUG

/* template T
   class vec
   {
   vec();
   vec(const& vec);

   operator= (const @vec);
   virtual ~vec();

   void dump() { for (auto e : _v) cout << e << " "; cout<<endl;}

   private:
   vector<T> _v;
   }*/

// A utility function to swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void dump(const std::vector<int> & data)
{
#ifdef DEBUG
    int i=0;
    for (auto v:data) {
        if ((i % 10) == 0) std::cout<<std::endl;
        std::cout<< v <<" ";
        ++i;
    }
    std::cout<<std::endl;
#endif
}

int Partition(std::vector<int> & data, int low, int high)
{
    int p = low;
    for (auto i = p+1; i < data.size(); ++i)
    {
        if (data[i] < data[p])
        {
            swap(&data[i], &data[p]);
            if (i != p+1)
            {
                swap(&data[i], &data[p+1]);
            }
            p = p + 1;
        }
    }
    //std::cout << "p: " << p << "    ";
    //dump(data);
    return p;
}

void StdSort(std::vector<int> & data, int low, int high)
{
    std::sort(data.begin(), data.end());
}

void QuickSort(std::vector<int> & data, int low, int high)
{
    if (low >= high) return;

    int p = Partition(data, low, high);

    QuickSort(data, low, p-1);
    QuickSort(data, p+1, high);
}

void bench(void (*f)(std::vector<int> &, int , int ), std::vector<int> &v)
{
    dump(v);
    //std::cout << "size: " << v.size() << std::endl;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    f(v, 0, v.size());

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    dump(v);
    std::cout << "Finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

static std::vector<int> generate_data(size_t size)
{
    static std::uniform_int_distribution<int> distribution(0, 1000);
    static std::default_random_engine generator;

    std::vector<int> data(size);
    std::generate(data.begin(), data.end(), []() { return distribution(generator); });
    return data;
}

int main(int argc, char *argv[])
{
    size_t vec_size = 10;
    if (argc == 2) {
        vec_size = atoi(argv[1]);
    } else if (argc > 2) {
        std::cout<<"Usage:"<<std::endl<<"      "<<argv[0] << " <size>"<<std::endl;
        exit(1);
    }
    //std::vector<int> v({ 6, 4, 9, 10,  50, 48, 35, 56, 1, 5, 2, 90, 100 });
    std::vector<int> v(generate_data(vec_size));
    std::vector<int> v1(v);

    std::cout<<std::endl<<"QuickSort implementation (size: "<<vec_size<<"): "<<std::endl;
    bench(&QuickSort, v);
    std::cout<<std::endl<<"std::sort implementation (size: "<<vec_size<<"): "<<std::endl;
    bench(&StdSort, v1);

    std::cout<<"Exit"<<std::endl;
}
