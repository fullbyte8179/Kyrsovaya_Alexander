#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <iomanip>

using namespace std;

// --- КОНСТАНТЫ ---
const double CAP_RATE = 0.07;    // Ставка капитализации (7%)
const double COST_COEFF = 0.3;   // Коэффициент затрат (30%)

// --- СТРУКТУРЫ ---
struct CalculationResult {
    double cadastral_value;
    double rent_score;
    double market_value;
    double raw_rent;
};

struct TestCase {
    string name;
    string desc;
    double d[5];
    bool expect_success;
};

// --- ИНТЕРФЕЙС ---
void print_header() {
    cout << "\n\n";
    cout << "  ==================================================\n";
    cout << "  |   СИСТЕМА КАДАСТРОВОЙ ОЦЕНКИ ЗЕМЕЛЬ (СКЗ)      |\n";
    cout << "  |       (Вариант 134 | Разработал: Аракчеев)     |\n";
    cout << "  ==================================================\n";
}

void print_separator() {
    cout << "  --------------------------------------------------\n";
}

void print_tooltip(string title, string info) {
    cout << "\n  [" << title << "]\n";
    cout << "  Инфо: " << info << "\n";
}

void pause_console() {
    cout << "\n  >> Нажмите Enter для продолжения...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- ВВОД (ЗАЩИТА ОТ ОШИБОК) ---
double get_secure_input(string prompt, double min, double max) {
    double value;
    while (true) {
        cout << "  > Введите " << prompt << ": ";
        
        if (cin >> value) {
            // Проверка на мусор после числа
            if (cin.peek() != '\n') {
                cout << "    [ОШИБКА] Обнаружены буквы! Введите только цифры.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            // Проверка диапазона
            if (value >= min && value <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << "    [ОШИБКА] Допустимый диапазон: " << min << " ... " << max << "\n";
            }
        } else {
            // Если ввели текст
            cout << "    [ОШИБКА] Это не число. Попробуйте снова.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// --- РАСЧЕТ ---
CalculationResult calculate(double area, double bonitet, double income, double loc_k, double market_k) {
    CalculationResult res;
    
    // Рента
    double rent = (income * (bonitet / 100.0)) - (income * COST_COEFF);
    
    // Защита от минуса
    if (rent < 0) res.raw_rent = 0;
    else res.raw_rent = rent;

    // Капитализация
    double adj_rent = res.raw_rent * loc_k;
    res.cadastral_value = (adj_rent / CAP_RATE) * area;
    res.market_value = res.cadastral_value * market_k;
    
    // Эффективность
    if (income > 0) res.rent_score = adj_rent / income;
    else res.rent_score = 0;

    return res;
}

// --- ВЕРДИКТ ---
void print_verdict(CalculationResult res) {
    cout << "\n  --- ЭКСПЕРТНОЕ ЗАКЛЮЧЕНИЕ ---\n";
    if (res.raw_rent == 0) cout << "  СТАТУС: УБЫТОЧНЫЙ ОБЪЕКТ (Неликвид).\n  Совет: Затраты превышают доход.\n";
    else if (res.rent_score < 0.5) cout << "  СТАТУС: НИЗКАЯ ЭФФЕКТИВНОСТЬ.\n  Совет: Требуются вложения.\n";
    else if (res.rent_score < 1.0) cout << "  СТАТУС: СРЕДНЯЯ ДОХОДНОСТЬ.\n  Совет: Стабильный актив.\n";
    else cout << "  СТАТУС: ВЫСОКОДОХОДНЫЙ АКТИВ!\n  Совет: Отличная инвестиция.\n";
}

// --- ДИАГНОСТИКА ---
bool run_diagnostics() {
    print_header();
    cout << "  [РЕЖИМ АВТОМАТИЧЕСКОЙ ДИАГНОСТИКИ]\n\n";
    cout << "  Цель модуля: Проверка математического ядра на эталонах.\n";
    cout << "  Мы проверим 5 сценариев (от идеального до убыточного).\n";
    print_separator();
    
    cout << "  Нажмите 1 для СТАРТА или 0 для ВОЗВРАТА В МЕНЮ.\n";
    
    int choice = (int)get_secure_input("Ваш выбор", 0, 1);

    if (choice == 0) return true;

    cout << "\n  >>> ЗАПУСК ТЕСТОВ...\n";
    
    vector<TestCase> db = {
        {"ЗОЛОТАЯ ЖИЛА", "Идеальный участок (Bon=90), высокий доход", {100.0, 90.0, 60000.0, 1.0, 1.0}, true},
        {"СРЕДНЯЧОК", "Обычный участок, средние показатели", {50.0, 60.0, 30000.0, 1.0, 1.0}, true},
        {"УБЫТОЧНЫЙ", "Низкий бонитет (20), затраты > дохода", {10.0, 20.0, 10000.0, 1.0, 1.0}, false},
        {"УДАЛЕННЫЙ", "Хорошая земля, плохая логистика (Loc=0.5)", {100.0, 80.0, 40000.0, 0.5, 1.0}, true},
        {"СПЕКУЛЯТИВНЫЙ", "Средняя земля, высокий спрос (Mark=2.0)", {100.0, 60.0, 40000.0, 1.0, 2.0}, true}
    };

    cout << fixed << setprecision(2);
    int passed = 0;

    for (const auto& t : db) {
        cout << "\n  --- ТЕСТ: " << t.name << " ---\n";
        cout << "  Описание: " << t.desc << "\n";
        
        CalculationResult r = calculate(t.d[0], t.d[1], t.d[2], t.d[3], t.d[4]);
        cout << "  >> Кадастровая стоимость: " << r.cadastral_value << " руб.\n";

        bool ok = false;
        if (t.expect_success && r.cadastral_value > 0) ok = true;
        if (!t.expect_success && r.cadastral_value == 0) ok = true;

        if (ok) {
            cout << "  ВЕРДИКТ: [УСПЕХ]\n";
            passed++;
        } else {
            cout << "  ВЕРДИКТ: [СБОЙ]\n";
        }
    }
    print_separator();
    cout << "  РЕЗУЛЬТАТ: Пройдено " << passed << " из 5 тестов.\n";
    print_separator();

    cout << "  1. Вернуться в Главное Меню\n";
    cout << "  2. Выйти из программы\n";
    
    int post_choice = (int)get_secure_input("Ваш выбор", 1, 2);
    
    if (post_choice == 1) return true;
    else return false;
}

// --- РАСЧЕТ ---
void run_calculation_mode() {
    print_header();
    cout << "  [РЕЖИМ РАСЧЕТА СТОИМОСТИ]\n";
    print_separator();

    print_tooltip("ПЛОЩАДЬ", "Размер участка в гектарах (например, 100).");
    double s = get_secure_input("площадь", 0.01, 1e6);

    print_tooltip("БОНИТЕТ", "Качество почвы (0-100). 100 - чернозем.");
    double b = get_secure_input("бонитет", 0.0, 100.0);

    print_tooltip("ДОХОД", "Выручка с 1 га (грязными, в рублях).");
    double i = get_secure_input("доход", 0.0, 1e9);

    print_tooltip("ЛОКАЦИЯ", "Коэф. инфраструктуры. 1.0 - норма, <1 - глушь.");
    double k1 = get_secure_input("коэф. локации", 0.1, 2.0);

    print_tooltip("РЫНОК", "Коэф. спроса. 1.0 - баланс, >1 - дефицит.");
    double k2 = get_secure_input("рыночный коэф.", 0.5, 3.0);

    CalculationResult res = calculate(s, b, i, k1, k2);
    
    print_separator();
    cout << "  ФИНАНСОВЫЙ ОТЧЕТ:\n";
    cout << fixed << setprecision(2);
    cout << "  1. Кадастровая стоимость: " << res.cadastral_value << " руб.\n";
    cout << "  2. Рыночная стоимость:    " << res.market_value << " руб.\n";
    print_verdict(res);
    print_separator();
    
    pause_console();
}

// --- MAIN ---
int main() {
    setlocale(LC_ALL, "Russian");
    bool active = true;

    while (active) {
        print_header();
        cout << "  1. Начать расчет\n";
        cout << "  2. Полная диагностика\n";
        cout << "  3. Выход\n";
        print_separator();
        
        int choice = (int)get_secure_input("номер пункта", 1, 3);

        switch (choice) {
            case 1:
                run_calculation_mode();
                break;
            case 2:
                if (!run_diagnostics()) active = false;
                break;
            case 3:
                cout << "\n  До свидания!\n";
                active = false;
                break;
        }
    }
    return 0;
}
