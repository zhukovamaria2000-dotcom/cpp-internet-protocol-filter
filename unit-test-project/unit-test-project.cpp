#include "pch.h"
#include "CppUnitTest.h"
#include "../main-project/Session.h"
#include "../main-project/Processing.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittestproject
{
	TEST_CLASS(unittestproject)
	{
	public:
		
        TEST_METHOD(TestTotalDurationForSkype)
        {
            //Подготовка данных
            vector<Session> sessions = {
                {0, 1800, 1000, 500, "C:\\Skype.exe"},        // 30 минут = 1800 сек
                {1800, 3600, 2000, 1000, "C:\\Skype.exe"},    
                {3600, 5400, 3000, 1500, "C:\\Chrome.exe"}    // 30 минут, но не Skype
            };

            // Выполнение
            long long result = getTotalDurationForProgram(sessions, "Skype");

            //Проверка
            Assert::AreEqual(3600LL, result);  // 1800 + 1800 = 3600
        }

        // Тест 2: Программа не найдена
        TEST_METHOD(TestTotalDurationForNotFoundProgram)
        {
            vector<Session> sessions = {
                {0, 1800, 1000, 500, "C:\\Skype.exe"},
                {1800, 3600, 2000, 1000, "C:\\Chrome.exe"}
            };

            long long result = getTotalDurationForProgram(sessions, "Firefox");

            Assert::AreEqual(0LL, result);
        }

        // Тест 3: Пустой список сеансов
        TEST_METHOD(TestTotalDurationForEmptySessions)
        {
            vector<Session> sessions;

            long long result = getTotalDurationForProgram(sessions, "Skype");

            Assert::AreEqual(0LL, result);
        }

        // Тест 4: Убеждаемся, что другие программы не влияют на результат
        TEST_METHOD(TestTotalDurationIgnoresOtherPrograms)
        {
            vector<Session> sessions = {
                {0, 1800, 1000, 500, "C:\\Skype\\Skype.exe"},
                {1800, 3600, 2000, 1000, "C:\\Zoom\\Zoom.exe"},
                {3600, 4000, 3000, 1500, "C:\\Discord\\Discord.exe"},
                {4000, 5000, 4000, 2000, "C:\\Skype\\Skype.exe"}
            };

            long long result = getTotalDurationForProgram(sessions, "Skype");

            // Считаем только Skype
            Assert::AreEqual(2800LL, result);
        }
	};
}
