#include "collatzsolver.h"

CollatzSolver::CollatzSolver(QObject* parent ) : QObject(parent) {}

void CollatzSolver::SetThreadsCount(int newThreadsCount)
{
    threadsCount = newThreadsCount;
}

void CollatzSolver::SetLimit(size_t newLimit)
{
    limit = newLimit;
}



size_t CollatzSolver::Length(size_t n)
{
    int length=1;

    if ((n & (n - 1)) == 0)
    {
        length += log2(n);
        return length;
    }
    else
    {
    while(n!=1)
    {
        if(n%2!=0)
        {
            n = ((n * 3) + 1) / 2;
            length+=2;
        }
        else
            {
                n/=2;
                length++;
            }
    }
    return length;
    }
}

void CollatzSolver::ThreadsSetup()
{
    threads.clear();
    threads.resize(threadsCount);
    if(threadsCount == 1)
    {
        threads[0] = std::thread(&CollatzSolver::FindLongestSequence, this, 1, limit);
    }
    else
    {
        size_t partLength = limit / threadsCount;

        size_t odd = limit % threadsCount;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            size_t start = i * partLength;

            size_t end = (i + 1) * partLength;
            if(i==0)
            {
                start++;
            }

            if (i == threadsCount - 1 && odd != 0) end += odd;

            threads[i] = std::thread(&CollatzSolver::FindLongestSequence, this, start, end);
        }
    }
    for (auto &t : threads)
    {
        t.detach();
    }
}

void CollatzSolver::FindLongestSequence(int start, int end)
{
    size_t localLongest = 0;
    size_t localNumber = 0;

    if (stopFlag.load())
        return;
    for (int i = start; i <= end; i++)
    {

        size_t length;
        length = Length(i);
        if (length > localLongest)
        {
            localLongest = length;
            localNumber = i;
        }
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (localLongest > longestSequence)
        {
            longestSequence = localLongest;
            numberWithLongestSequence = localNumber;
        }
    }
    if (++completedThreads == threadsCount)
    {
        completedThreads = 0;
        Stop();
    }
}
void CollatzSolver::Start()
{
    stopFlag.store(false);
    start = std::chrono::high_resolution_clock::now();
    ThreadsSetup();
}

void CollatzSolver::Stop()
{
    auto end = std::chrono::high_resolution_clock::now();
    qDebug() << "Start time: " << std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() << "ms";
    qDebug() << "End time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "ms";
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    qDebug() << time.count();
    stopFlag.store(true);
    emit stopSignal();
}


size_t CollatzSolver::GetLongestSequence() const
{
    return longestSequence;
}

size_t CollatzSolver::GetNumberWithLongestSequence() const
{
    return numberWithLongestSequence;
}

std::chrono::milliseconds CollatzSolver::GetTime() const
{
    return time;
}


void CollatzSolver::SetLongestSequence(size_t newLongestSequence)
{
    longestSequence = newLongestSequence;
}

void CollatzSolver::SetNumberWithLongestSequence(size_t newNumberWithLongestSequence)
{
    numberWithLongestSequence = newNumberWithLongestSequence;
}

void CollatzSolver::setTime(const std::chrono::milliseconds &newTime)
{
    time = newTime;
}


