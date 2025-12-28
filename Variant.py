import sys


CAP_RATE = 0.07      # Ставка капитализации 7%
COST_COEFF = 0.3     # Затраты 30%



def print_header():
    print("\n\n")
    print("  ==================================================")
    print("  |   СИСТЕМА КАДАСТРОВОЙ ОЦЕНКИ ЗЕМЕЛЬ (СКЗ)      |")
    print("  |       (Вариант 134 | Разработал: Аракчеев)     |")
    print("  ==================================================")

def print_separator():
    print("  --------------------------------------------------")

def print_tooltip(title, info):
    print(f"\n  [{title}]")
    print(f"  Инфо: {info}")

def pause_console():
    input("\n  >> Нажмите Enter для продолжения...")



def get_secure_input(prompt, min_val, max_val):
    """
    Ввод с защитой от ошибок (без таймера).
    """
    while True:
        raw = input(f"  > Введите {prompt}: ")
        try:
           
            val = float(raw.replace(',', '.'))
            
            if min_val <= val <= max_val:
                return val
            else:
                print(f"    [ОШИБКА] Допустимый диапазон: {min_val} ... {max_val}")
        except ValueError:
            print("    [ОШИБКА] Это не число. Попробуйте снова.")



def calculate(area, bonitet, income, loc_k, market_k):
    
    rent = (income * (bonitet / 100.0)) - (income * COST_COEFF)
    
    
    raw_rent = rent if rent >= 0 else 0

    
    adj_rent = raw_rent * loc_k
    cadastral = (adj_rent / CAP_RATE) * area

    
    market = cadastral * market_k
    
    
    if income > 0:
        rent_score = adj_rent / income
    else:
        rent_score = 0
        
    return {
        "cadastral": cadastral,
        "market": market,
        "score": rent_score,
        "raw_rent": raw_rent
    }

def print_verdict(res):
    print("\n  --- ЭКСПЕРТНОЕ ЗАКЛЮЧЕНИЕ ---")
    if res["raw_rent"] == 0:
        print("  СТАТУС: УБЫТОЧНЫЙ ОБЪЕКТ (Неликвид).")
        print("  Совет: Затраты превышают доход.")
    elif res["score"] < 0.5:
        print("  СТАТУС: НИЗКАЯ ЭФФЕКТИВНОСТЬ.")
        print("  Совет: Требуются вложения.")
    elif res["score"] < 1.0:
        print("  СТАТУС: СРЕДНЯЯ ДОХОДНОСТЬ.")
        print("  Совет: Стабильный актив.")
    else:
        print("  СТАТУС: ВЫСОКОДОХОДНЫЙ АКТИВ!")
        print("  Совет: Отличная инвестиция.")



def run_diagnostics():
    print_header()
    print("  [РЕЖИМ АВТОМАТИЧЕСКОЙ ДИАГНОСТИКИ]\n")
    print("  Цель модуля: Проверка математического ядра на эталонах.")
    print("  Мы проверим 5 сценариев (от идеального до убыточного).")
    print_separator()
    
    print("  Нажмите 1 для СТАРТА или 0 для ВОЗВРАТА В МЕНЮ.")
    
    while True:
        choice = input("  > Ваш выбор: ")
        if choice == '1' or choice == '0': break
        print("    Введите 1 или 0!")
        
    if choice == '0': return True # Возврат в меню

    print("\n  >>> ЗАПУСК ТЕСТОВ...")
    
    
    tests = [
        ("ЗОЛОТАЯ ЖИЛА", "Идеальный участок, высокий доход", 
         [100.0, 90.0, 60000.0, 1.0, 1.0], True),
         
        ("СРЕДНЯЧОК", "Обычный участок, средние показатели", 
         [50.0, 60.0, 30000.0, 1.0, 1.0], True),
         
        ("УБЫТОЧНЫЙ", "Низкий бонитет, затраты > дохода", 
         [10.0, 20.0, 10000.0, 1.0, 1.0], False),
         
        ("УДАЛЕННЫЙ", "Хорошая земля, плохая логистика", 
         [100.0, 80.0, 40000.0, 0.5, 1.0], True),
         
        ("СПЕКУЛЯТИВНЫЙ", "Средняя земля, высокий спрос", 
         [100.0, 60.0, 40000.0, 1.0, 2.0], True)
    ]

    passed = 0
    
    for t in tests:
        name, desc, d, expect_success = t
        print(f"\n  --- ТЕСТ: {name} ---")
        print(f"  Описание: {desc}")
        
        r = calculate(d[0], d[1], d[2], d[3], d[4])
        print(f"  >> Кадастровая стоимость: {r['cadastral']:.2f} руб.")
        
        ok = False
        if expect_success and r['cadastral'] > 0: ok = True
        if not expect_success and r['cadastral'] == 0: ok = True
        
        if ok:
            print("  ВЕРДИКТ: [УСПЕХ]")
            passed += 1
        else:
            print("  ВЕРДИКТ: [СБОЙ]")
            
    print_separator()
    print(f"  РЕЗУЛЬТАТ: Пройдено {passed} из 5 тестов.")
    print_separator()
    
    
    print("  1. Вернуться в Главное Меню")
    print("  2. Выйти из программы")
    
    while True:
        post_choice = input("  > Ваш выбор: ")
        if post_choice == '1': return True
        if post_choice == '2': return False
        print("    Введите 1 или 2!")



def run_calculation_mode():
    print_header()
    print("  [РЕЖИМ РАСЧЕТА СТОИМОСТИ]")
    print_separator()

    # Ввод
    print_tooltip("ПЛОЩАДЬ", "Размер участка в гектарах.")
    s = get_secure_input("площадь", 0.01, 1e6)

    print_tooltip("БОНИТЕТ", "Качество почвы (0-100).")
    b = get_secure_input("бонитет", 0.0, 100.0)

    print_tooltip("ДОХОД", "Выручка с 1 га (руб).")
    i = get_secure_input("доход", 0.0, 1e9)

    print_tooltip("ЛОКАЦИЯ", "Коэф. инфраструктуры (0.1 - 2.0).")
    k1 = get_secure_input("коэф. локации", 0.1, 2.0)

    print_tooltip("РЫНОК", "Коэф. спроса (0.5 - 3.0).")
    k2 = get_secure_input("рыночный коэф.", 0.5, 3.0)

    
    res = calculate(s, b, i, k1, k2)

    print_separator()
    print("  ФИНАНСОВЫЙ ОТЧЕТ:")
    print(f"  1. Кадастровая стоимость:  {res['cadastral']:.2f} руб.")
    print(f"  2. Рыночная стоимость:     {res['market']:.2f} руб.")
    print(f"  3. Эффективность:          {res['score']:.4f}")
    
    print_verdict(res)
    print_separator()
    
    pause_console()



def main():
    active = True
    while active:
        print_header()
        print("  1. Начать расчет")
        print("  2. Полная диагностика")
        print("  3. Выход")
        print_separator()
        
        choice = get_secure_input("номер пункта", 1, 3)
        
        if choice == 1:
            run_calculation_mode()
        elif choice == 2:
            if not run_diagnostics(): active = False
        elif choice == 3:
            print("\n  До свидания!")
            active = False

if __name__ == "__main__":
    main()
