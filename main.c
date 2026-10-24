#include <stdio.h>  // Підключення стандартної бібліотеки вводу-виводу (для printf, scanf)
#include <stdlib.h> // Підключення загальної бібліотеки (для system("cls") тощо)
#include <math.h>   // Підключення математичної бібліотеки (для pow, fabs)

/* =========================================================================
 * ПРОТОТИПИ ФУНКЦІЙ
 * Попереднє оголошення назв функцій та їхніх типів, щоб компілятор знав про
 * їхнє існування до того, як вони будуть викликані у функції main.
 * ========================================================================= */
double num_comput_integral_l_re(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double num_comput_integral_r_re(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double num_comput_integral_trap(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double num_comput_integral_Simps(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double integrand_expression(double x); // Прототип підінтегральної функції (Варіант 18: f(x) = x^2)

int main()
{
    // Оголошення та ініціалізація змінних для меж інтегрування
    double left_boundary_a = 0;  // Змінна для зберігання лівої межі [a] (X перше)
    double right_boundary_b = 0; // Змінна для зберігання правої межі [b] (X останнє)

    // Оголошення змінних для обчислення похибки та значень інтегралів
    double measurement_error = 0; // Задана користувачем допустима похибка (епсилон)
    double I1 = 0;                // Змінна для значення інтеграла з N проміжками
    double I2 = 0;                // Змінна для значення інтеграла з N+2 проміжками

    // Оголошення службових змінних
    int intervals = 0;   // Змінна для зберігання кількості проміжків розбиття (n)
    int var = 0;         // Змінна для вибору пункту меню (номер методу розрахунку)
    double integral_s = 0; // Змінна для накопичення та зберігання результату обчислення інтеграла

    // Головний цикл програми (while(1)) — забезпечує багаторазове виконання розрахунків без перезапуску програми
    while(1)
    {
        // Виведення запиту на введення лівої межі інтегрування
        printf("\n\nEnter the left boundary \n X(first)=");
        // Зчитування введенкового числа з клавіатури у змінну left_boundary_a (якщо сталася помилка введення - перериваємо цикл)
        if (scanf("%lf", &left_boundary_a) != 1) break;

        // Виведення запиту на введення правої межі інтегрування
        printf("\nEnter the right boundary \n X(last)=");
        // Зчитування правої межі у змінну right_boundary_b
        scanf("%lf", &right_boundary_b);

        // Цикл з передумовою (do-while) для перевірки коректності кількості проміжків (N має бути більше за 0)
        do {
            printf("\nEnter the number of partition intervals (N>0)\nN=");
            scanf("%u", &intervals); // Зчитування кількості проміжків як неякого цілого числа
        } while(intervals <= 0); // Якщо N <= 0, цикл повторюється знову

        // Введення значення допустимої похибки (епсилон) для автоматичного підбору кроку
        printf("\nEnter the measurement error of integration\n Measurement error=");
        scanf("%lf", &measurement_error);

        // Цикл введення та перевірки вибору чисельного методу (меню програми)
        do {
            printf("\nChoose the method of calculating:\n");
            printf("\t1. By Left Rectangles\n");
            printf("\t2. By Right Rectangles\n");
            printf("\t3. By Trapezoids\n");
            printf("\t4. By Parabola method (Simpson)\n");
            scanf("%u", &var); // Зчитування вибору користувача (1, 2, 3 або 4)

            // Валідація: якщо введено число поза діапазоном 1-4, виводимо повідомлення про помилку
            if (var < 1 || var > 4)
                printf("\nYou are mistaken\n");
        } while(var < 1 || var > 4); // Повторювати, поки вибір не стане правильним

        system("cls"); // Системна команда очищення консольного екрана перед виведенням результатів

        // Оператор розгалуження (switch-case) за значенням змінної вибору методу (var)
        switch(var)
        {
            case 1: // Випадок 1: Метод лівих прямокутників
            {
                // Виклик функції лівих прямокутників, передаємо межі та кількість проміжків
                integral_s = num_comput_integral_l_re(left_boundary_a, right_boundary_b, intervals);
                // Виведення результатів обчислення на екран
                printf("\n\n\tLeft Rectangles method\n");
                printf("\na = %.2lf \t b = %.2lf \t Integral = %.8lf \t N = %d\n",
                       left_boundary_a, right_boundary_b, integral_s, intervals);
                break; // Вихід із гілки switch
            }
            case 2: // Випадок 2: Метод правих прямокутників (з підбором кроку за похибкою)
            {
                printf("\n\n\t======* Right Rectangles method *======\n");
                unsigned int current_n = intervals; // Поточна кількість відрізків
                // Первинне обчислення інтеграла I1 для заданої кількості проміжків
                I1 = num_comput_integral_r_re(left_boundary_a, right_boundary_b, current_n);

                // Нескінченний цикл уточнення кількості відрізків за заданою точністю
                while(1) {
                    // Обчислення значення інтеграла I2 для збільшеної кількості відрізків (current_n + 2)
                    I2 = num_comput_integral_r_re(left_boundary_a, right_boundary_b, current_n + 2);
                    // Перевірка умови досягнення заданої похибки (модуль різниці I1 та I2 <= похибки)
                    if (fabs(I1 - I2) <= measurement_error) {
                        current_n += 2; // Фіксуємо фінальну кількість відрізків
                        I1 = I2;        // Оновлюємо значення
                        break;          // Виходимо з циклу підбору
                    }
                    current_n += 2; // Збільшуємо кількість відрізків на 2, якщо точність ще не досягнута
                    I1 = I2;        // Зберігаємо поточне значення як попереднє
                }
                // Виведення остаточних результатів після підбору
                printf("\na = %.2lf \t b = %.2lf \t Integral = %.8lf \t N = %u\n",
                       left_boundary_a, right_boundary_b, I2, current_n);
                break;
            }
            case 3: // Випадок 3: Метод трапецій
            {
                // Виклик функції обчислення методом трапецій
                integral_s = num_comput_integral_trap(left_boundary_a, right_boundary_b, intervals);
                printf("\n\n\t======* Trapezoids method *======\n");
                printf("\na = %.2lf \t b = %.2lf \t Integral = %.8lf \t N = %d\n",
                       left_boundary_a, right_boundary_b, integral_s, intervals);
                break;
            }
            case 4: // Випадок 4: Метод парабол (Сімпсона)
            {
                // Виклик функції обчислення методом Сімпсона
                integral_s = num_comput_integral_Simps(left_boundary_a, right_boundary_b, intervals);
                printf("\n\n\t======* Simpson's method (Parabola) *======\n");
                printf("\na = %.2lf \t b = %.2lf \t Integral = %.8lf \t N = %d\n",
                       left_boundary_a, right_boundary_b, integral_s, intervals);
                break;
            }
        }

        // Опитування користувача про продовження роботи програми
        char choice;
        printf("\nDo you want to continue? (y/n): ");
        scanf(" %c", &choice); // Зчитування символу відповіді
        if (choice == 'n' || choice == 'N') break; // Якщо введено 'n', виходимо з головного циклу
    }

    return 0; // Завершення роботи програми успішно
}


/* =========================================================================
 * ФУНКЦІЯ 1: МЕТОД ЛІВИХ ПРЯМОКУТНИКІВ
 * Призначення: обчислює визначений інтеграл за формулою лівих прямокутників.
 * ========================================================================= */
double num_comput_integral_l_re(double left_boundary_a, double right_boundary_b, unsigned int intervals)
{
    // Обчислення кроку інтегрування h: довжина відрізка поділена на кількість проміжків
    double h = (right_boundary_b - left_boundary_a) / intervals;
    double integral_s = 0; // Змінна для сумування площ прямокутників
    double x = left_boundary_a; // Поточна координата x починається з лівої межі a

    // Цикл підсумовування значень функції на лівих кінцях відрізків (від 0 до n-1)
    for (unsigned int i = 0; i < intervals; i++)
    {
        integral_s += integrand_expression(x); // Додаємо значення функції у точці x
        x += h; // Зсуваємо координату x на один крок вперед
    }
    return integral_s * h; // Множимо отриману суму на крок h для отримання площі
}


/* =========================================================================
 * ФУНКЦІЯ 2: МЕТОД ПРАВИХ ПРЯМОКУТНИКІВ
 * Призначення: обчислює інтеграл за значеннями функції на правих кінцях відрізків.
 * ========================================================================= */
double num_comput_integral_r_re(double left_boundary_a, double right_boundary_b, unsigned int intervals)
{
    // Обчислення кроку h
    double h = (right_boundary_b - left_boundary_a) / intervals;
    double integral_s = 0;
    double x = left_boundary_a;

    // Цикл підсумовування від 1 до n (використовує праві кінці проміжків)
    for (unsigned int i = 1; i <= intervals; i++)
    {
        x = left_boundary_a + i * h; // Обчислення правої координати для кроку i
        integral_s += integrand_expression(x); // Додавання значення функції
    }
    return integral_s * h; // Множення суми на крок h
}


/* =================================================0========================
 * ФУНКЦІЯ 3: МЕТОД ТРАПЕЦІЙ
 * Призначення: апроксимує площу під кривою за допомогою трапецій.
 * ========================================================================= */
double num_comput_integral_trap(double left_boundary_a, double right_boundary_b, unsigned int intervals)
{
    // Обчислення кроку h
    double h = (right_boundary_b - left_boundary_a) / intervals;
    double sum = 0; // Змінна для суми внутрішніх вузлів

    // Цикл обчислення суми значень функції у внутрішніх вузлах (від 1 до n-1)
    for (unsigned int i = 1; i < intervals; i++)
    {
        double x = left_boundary_a + i * h;
        sum += integrand_expression(x);
    }
    // Повернення результату за формулою трапецій: (h/2) * (f(a) + f(b) + 2 * сума_внутрішніх)
    return (h / 2.0) * (integrand_expression(left_boundary_a) + integrand_expression(right_boundary_b) + 2.0 * sum);
}


/* =========================================================================
 * ФУНКЦІЯ 4: МЕТОД ПАРАБОЛ (СІМПСОНА)
 * Призначення: чисельне інтегрування за допомогою квадратичної інтерполяції.
 * ========================================================================= */
double num_comput_integral_Simps(double left_boundary_a, double right_boundary_b, unsigned int intervals)
{
    // Метод Сімпсона вимагає парної кількості проміжків. Якщо введено непарне N, збільшуємо його на 1.
    if (intervals % 2 != 0) intervals++;

    // Обчислення кроку h
    double h = (right_boundary_b - left_boundary_a) / intervals;
    double sum_odd = 0;   // Змінна для суми з непарними індексами (множаться на 4)
    double sum_even = 0;  // Змінна для суми з парними індексами (множаться на 2)

    // Цикл обчислення сум для внутрішніх вузлів
    for (unsigned int i = 1; i < intervals; i++)
    {
        double x = left_boundary_a + i * h;
        if (i % 2 == 0)
            sum_even += integrand_expression(x); // Якщо індекс парний
        else
            sum_odd += integrand_expression(x);  // Якщо індекс непарний
    }
    // Повернення результату за формулою Сімпсона: (h/3) * (f(a) + f(b) + 4*сума_непарних + 2*сума_парних)
    return (h / 3.0) * (integrand_expression(left_boundary_a) + integrand_expression(right_boundary_b) + 4.0 * sum_odd + 2.0 * sum_even);
}


/* =========================================================================
 * ПІДІНТЕГРАЛЬНА ФУНКЦІЯ (ВААРІАНТ 18)
 * Призначення: повертає значення математичної функції f(x) для заданого x.
 * Для Варіанта 18 це функція: f(x) = x^2.
 * ========================================================================= */
double integrand_expression(double x)
{
    return pow(x, 2); // Використовує стандартну функцію піднесення до степеня pow(основа, степінь)
}


