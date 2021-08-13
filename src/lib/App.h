#ifndef _APP_H_
#define _APP_H_

#ifdef _WIN32
   #include <windows.h>
   /** 
     * \brief Estrutura para tratar cronômetro.
     */
   struct stStopwatch_Win
   {
      LARGE_INTEGER mStartTime;
      LARGE_INTEGER mEndTime;
      double mCPUFreq;
      double mElapsedTime;
   };
   typedef struct stStopwatch_Win Stopwatch;

   /** 
     * \brief Inicializa cronômetro.
     */
   #define FREQUENCY( prm ) {                                                    \
      LARGE_INTEGER CPU_freq;                                                    \
      double mCPUFreq  ;                                                         \
      QueryPerformanceFrequency((LARGE_INTEGER*) &CPU_freq);                     \
      mCPUFreq = static_cast <double> (CPU_freq.QuadPart);                       \
      mCPUFreq /= 1000.0f;                                                       \
      QueryPerformanceCounter((LARGE_INTEGER*) &prm.mStartTime);                 \
      QueryPerformanceCounter((LARGE_INTEGER*) &prm.mEndTime);                   \
      prm.mElapsedTime = 0.0f;                                                   \
      prm.mCPUFreq = mCPUFreq;                                                   \
   } 


   /** 
     * \brief Dispara cronômetro.
     */
   #define START_STOPWATCH( prm ) {                                               \
      QueryPerformanceCounter((LARGE_INTEGER*) &prm.mStartTime);                 \
   }

   /** 
     * \brief Para cronômetro.
     */
   #define STOP_STOPWATCH( prm ) {                                                     \
      QueryPerformanceCounter((LARGE_INTEGER*) &prm.mEndTime);                         \
      prm.mElapsedTime = (double) (prm.mEndTime.QuadPart -  prm.mStartTime.QuadPart);  \
      prm.mElapsedTime /= prm.mCPUFreq;                                                \
   }
#else
   #include <time.h>
   #include <sys/time.h>
   /** 
     * \brief Estrutura para tratar cronômetro.
     */
   struct stStopwatch_Unix
   {
      struct timeval mStartTime;
      struct timeval mEndTime;
      double mCPUFreq;
      double mElapsedTime;
   };
   typedef struct stStopwatch_Unix Stopwatch;

   /** 
     * \brief Inicializa cronômetro.
     */
   #define FREQUENCY( prm ) {                                                    \
      prm.mCPUFreq = 0.0f;                                                       \
   } 


   /** 
     * \brief Dispara cronômetro.
     */
   #define START_STOPWATCH( prm ) {                                               \
       gettimeofday( &prm.mStartTime, 0);                                   \
   }

   /** 
     * \brief Para cronômetro.
     */
   #define STOP_STOPWATCH( prm ) {                                                     \
      gettimeofday( &prm.mEndTime, 0);                                                     \
      prm.mElapsedTime = (1000.0f * ( prm.mEndTime.tv_sec - prm.mStartTime.tv_sec) + (0.001f * (prm.mEndTime.tv_usec - prm.mStartTime.tv_usec)) );                                                \
   }

#endif

#endif
