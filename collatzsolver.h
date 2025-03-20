#ifndef COLLATZSOLVER_H
#define COLLATZSOLVER_H
#include <shared_mutex>
#include <thread>
#include <vector>
#include <QObject>
#include <QDebug>
class CollatzSolver : public QObject
{
    Q_OBJECT
public:

    explicit CollatzSolver(QObject *parent);
    void SetThreadsCount(int newThreadsCount);
    void SetLimit(size_t newLimit);
    size_t Length(size_t n);
    void ThreadsSetup();
    void FindLongestSequence(int start, int end);
    void Start();
    void Stop();
    size_t GetLongestSequence() const;
    size_t GetNumberWithLongestSequence() const;
    std::chrono::milliseconds GetTime() const;

    void SetLongestSequence(size_t newLongestSequence);
    void SetNumberWithLongestSequence(size_t newNumberWithLongestSequence);

    void setTime(const std::chrono::milliseconds &newTime);

signals:
    void stopSignal();
private:
    int threadsCount;
    size_t limit;
    size_t longestSequence;
    size_t numberWithLongestSequence;
    std::mutex mtx;
    std::vector<std::thread> threads;
    std::atomic<bool> stopFlag=false;
    std::atomic<int> completedThreads=0;
    std::chrono::milliseconds time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#endif // COLLATZSOLVER_H
