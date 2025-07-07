import subprocess

def run_test(expression, expected):
    try:
        process = subprocess.Popen(
            ['interpreter.exe'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        input_data = f"{expression}\nq\n"
        stdout, stderr = process.communicate(input_data)
        
        lines = stdout.split('\n')
        for line in lines:
            line = line.strip()
            if line.startswith('> ') and len(line) > 2:
                number_part = line[2:].strip()
                try:
                    result = float(number_part)
                    return result
                except ValueError:
                    if "Error" in number_part or "Invalid" in number_part:
                        return "ERROR"
        return None
    except:
        return None

def test_all():
    tests = [
        ("2+3*4-1", 13),
        ("20/4+3*2", 11),
        ("100-50/2+10", 85),
        ("-10+5*-2", -20),
        ("3*4+2*5-6/2", 19),
        ("48/6/2", 4),
        ("2*3*4", 24),
        ("1000000/8", 125000),
        ("-7*-3+1", 22),
        ("5+3*0", 5),
        ("50-25/5*2+8", 48),
        ("-15*-4/6+7*2", 24),
        ("200/8/5+3*4-2", 15),
    ]
    
    passed = 0
    
    print("Test Expression -> Expected | Actual | Result")
    print("-" * 45)
    
    for expr, expected in tests:
        result = run_test(expr, expected)
        
        if result is not None and abs(result - expected) < 0.0001:
            print(f"{expr:15} -> {expected:8} | {result:6} | PASS")
            passed += 1
        else:
            print(f"{expr:15} -> {expected:8} | {result:6} | FAIL")
    
    print(f"\nResults: {passed}/13 tests passed")

if __name__ == "__main__":
    test_all()