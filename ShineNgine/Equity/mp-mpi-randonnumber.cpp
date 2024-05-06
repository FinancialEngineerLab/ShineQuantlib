#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include <mutex>
#include <thread>
#include <future>
#include <omp.h>

#include <ql/quantlib.hpp>

using namespace QuantLib;
using namespace std::chrono;
 
std::mutex mtx;


void generatorSobolSequenceSingle_nomtx(SobolRsg& sobol,
                                  std::vector<std::vector<double> >& results,
                                  std::vector<Time> times, Size stepRows, Size sampleCols)
{
    InverseCumulativeRsg<SobolRsg, InverseCumulativeNormal> gsg(sobol);
    BrownianBridge bridge(times);
    
    std::vector<double> increments(stepRows);
    
        
    for(Size j = 0; j< sampleCols ;++j)
    {
        const std::vector<double> sample = gsg.nextSequence().value;
        bridge.transform(sample.begin(), sample.end(), increments.begin());
        
        for(Size i = 1; i < stepRows;++i)
        {
            double stepValue = increments[i];
            results[i][j] = stepValue;
            std::cout<<  " col (j path) " <<j<< " row(i step) " << i << " value:"<< results[i][j] << std::endl;
        }
    }

};



void generatorSobolSequenceSingle(SobolRsg& sobol,
                                  std::vector<std::vector<double> >& results,
                                  std::vector<Time> times, Size stepRows, Size sampleCols)
{
    InverseCumulativeRsg<SobolRsg, InverseCumulativeNormal> gsg(sobol);
    BrownianBridge bridge(times);
    
    std::vector<double> increments(stepRows);
    
        
    for(Size j = 0; j< sampleCols ;++j)
    {
        const std::vector<double> sample = gsg.nextSequence().value;
        bridge.transform(sample.begin(), sample.end(), increments.begin());
        
        for(Size i = 1; i < stepRows;++i)
        {
            //mtx.lock();
            std::lock_guard<std::mutex> guard(mtx);
            double stepValue = increments[i];
            results[i][j] = stepValue;
            std::cout<<  " col (j path) " <<j<< " row(i step) " << i << " value:"<< results[i][j] << std::endl;
            //mtx.unlock();
        }
    }

};




void generatorSobolSequenceSingle_openmp(SobolRsg& sobol,
                                  std::vector<std::vector<double> >& results,
                                  std::vector<Time> times, Size stepRows, Size sampleCols)
{
    InverseCumulativeRsg<SobolRsg, InverseCumulativeNormal> gsg(sobol);
    BrownianBridge bridge(times);
    
    std::vector<double> increments(stepRows);
    
        
    for(Size j = 0; j< sampleCols ;++j)
    {
        
        #pragma omp parallel for
        const std::vector<double> sample = gsg.nextSequence().value;
        bridge.transform(sample.begin(), sample.end(), increments.begin());
        
        for(Size i = 1; i < stepRows;++i)
        {
            //mtx.lock();
            std::lock_guard<std::mutex> guard(mtx);
            double stepValue = increments[i];
            results[i][j] = stepValue;
            std::cout<<  " col (j path) " <<j<< " row(i step) " << i << " value:"<< results[i][j] << std::endl;
            //mtx.unlock();
        }
    }

};






int main(int argc, const char * argv[])
{
    //std::vector<Time> times = { 0.1, 0.2 , 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2.0};
    std::vector<Time> times_10y = { 1.0/3600.0, 2.0/3600.0, 3.0 / 3600.0, 4.0 /3600.0};
    
    std::vector<double> times = { 0.1, 0.2 , 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2.0};
    
    double constant = 1.0 /3600.0;
    
    std::transform(times.begin(), times.end(), times.begin(),
                       [ constant ](double element) { return element * constant; });
    
    TimeGrid grid(times.begin(), times.end());
    TimeGrid grid_10y(times_10y.begin(), times_10y.end());
    
    Size N = times.size();
    Size N_10y = times_10y.size();
    
    Size samples = 20;
    
    unsigned long  seed = 19910405;
    SobolRsg sobol(N, seed);
    SobolRsg sobol_10y(N_10y, seed);
    
    InverseCumulativeRsg<SobolRsg, InverseCumulativeNormal> gsg(sobol);
    InverseCumulativeRsg<SobolRsg, InverseCumulativeNormal> gsg_10y(sobol_10y);
    
    
    BrownianBridge bridge(times);
    BrownianBridge bridge_10y(times_10y);
    
    
    // temp : Z something //
    std::vector<double> temp(N);
    std::vector<double> temp_10y(N_10y);
    
    std::vector<double> temp_browinan(N);
    std::vector<double> temp_browinan_10y(N_10y);
    
    // paths //
    std::vector<std::vector<double> > vec_double(N, std::vector<double>(samples));
    std::vector<std::vector<double> > vec_dw(N, std::vector<double>(samples));
    
    Matrix mat_rn(N, samples);
    Matrix mat_dw(N, samples);
    
    SequenceStatistics stats1(N);
    
    std::cout << "multi threading " << std::endl;
    
    Size numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    std::vector<std::vector<double> > temp_results(samples, std::vector<double>(N));
    
    
    std::cout << " test (no mutex )" << std::endl;
    
    std::vector<std::vector<double> > temp_results_nomtx(samples, std::vector<double>(N));
    auto start = high_resolution_clock::now();
    generatorSobolSequenceSingle_nomtx(sobol, temp_results_nomtx, times,N, samples);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    
    std::cout << std::endl;
    
    std::cout << " test (mutex O, no multi-threading )" << std::endl;

    std::vector<std::vector<double> > temp_results_test(samples, std::vector<double>(N));
     start = high_resolution_clock::now();
    generatorSobolSequenceSingle(sobol, temp_results_test, times,N, samples);
     stop = high_resolution_clock::now();
     duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "mutex O, Mutlti Threading inner thread ***" << std::endl;
    
    
    start =high_resolution_clock::now();
    //int pathsPerThread = samples / numThreads;
    for (int i = 0; i < numThreads; ++i)
    {
        //int start = i * pathsPerThread;
        //int end = (i == numThreads - 1) ? samples : (i + 1) * pathsPerThread;
        threads.emplace_back(std::thread(generatorSobolSequenceSingle, std::ref(sobol), std::ref(temp_results),times, N, samples));
    }
    // Join threads
    for(auto& thread: threads)
    {
        thread.join();
    }
    //for(int i = 0; i< numThreads; ++i)
    //{
    //    threads.push_back(std::thread(generatorSobolSequenceSingle, std::ref(sobol), std::ref(temp_results),times, N, samples));
    //}
    
    // Wait for all threads to finish
    //std::for_each(threads.begin(), threads.end(), [](std::thread& t) { t.join(); });


    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    std::cout << std::endl;
    
    std::cout << " Multi Threaded Vector ! " << std::endl;
    for(Size j = 0; j < samples; j++)
    {
        for(Size i =0; i < N; i++)
        {
            std::cout << "col(j path) " << j << " row(i step) " << i << " value:" << temp_results[i][j] << "\n";
        }
    }
    
    std::cout << std::endl;
    std::cout << " test (openmp )" << std::endl;
    
    std::vector<std::vector<double> > temp_results_openmp(samples, std::vector<double>(N));
    start = high_resolution_clock::now();
    generatorSobolSequenceSingle_openmp(sobol, temp_results_openmp, times,N, samples);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    
    
    std::cout << std::endl;

    std::cout << " test (outer thread )" << std::endl;
    
    std::vector<std::vector<double> > temp_results_outthread(samples, std::vector<double>(N));
    start = high_resolution_clock::now();
    std::thread t(generatorSobolSequenceSingle, std::ref(sobol), std::ref(temp_results_outthread), times, N, samples);
    t.join();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    
    
    std::cout << std::endl;
    
    
    
    /*
    std::cout << " test (concurrency )" << std::endl;
    
    std::vector<std::vector<double> > temp_results_ccr(samples, std::vector<double>(N));
    start = high_resolution_clock::now();
    
    // Generate Brownian motion paths asynchronously
         std::future<std::vector<std::vector<double>>> future
    = std::async(std::launch::async, generatorSobolSequenceSingle,sobol,temp_results_ccr, times, N, samples);

    // Wait for the computation to finish and retrieve the result
    std::vector<std::vector<Real>> brownianPaths = future.get();

    
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "time comsuming : " <<duration.count() << std::endl;
    */
    
    /*
    std::vector<std::vector<double> > temp_results_tbl(samples, std::vector<double>(N));
    tbb::parallel_for(tbb::blocked_range<size_t>(0, samples),
                      generatorSobolSequenceSingle(sobol, temp_results_tbl,times, N, samples));
    
    
    std::cout << " Multi Threaded Vector ! " << std::endl;
    for(Size j = 0; j < samples; j++)
    {
        for(Size i =0; i < N; i++)
        {
            std::cout << "col(j path) " << j << " row(i step) " << i << " value:" << temp_results_tbl[i][j] << "\n";
        }
    }
    */
    /*
    Size numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    std::vector<std::vector<double> > temp_results(numThreads);
    for(Size i =0; i<numThreads;++i)
    {
        threads.push_back(std::thread(generatorSobolSequenceSingle, std::ref(sobol), std::ref(temp_results[i]), samples));
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& t) { t.join();});
    
    for(Size i = 0; i < numThreads; ++i)
    {
        std::cout << "Thread " << i << " : " << std::endl;
        for(Size row =0; row< N;++row)
        {
            
            std::cout << temp_results[][j] << " ";
        }
    }
     */
    return 0;
}
