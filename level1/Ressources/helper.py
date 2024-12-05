def create_pattern(length):
    """
    지정된 길이만큼 고유한 테스트 문자열을 생성합니다.
    """
    pattern = ""
    for i in range(1, 27):  # a-z 반복
        for j in range(1, 27):  # a-z 반복
            for k in range(10):  # 0-9 반복
                if len(pattern) < length:
                    pattern += chr(96 + i) + chr(96 + j) + str(k)
    return pattern[:length]


def find_offset(value, length):
    """
    생성된 패턴에서 특정 값의 오프셋을 찾습니다.
    """
    pattern = create_pattern(length)
    offset = pattern.find(value)
    if offset == -1:
        return f"'{value}' not found in the pattern!"
    return f"'{value}' found at offset: {offset}"


if __name__ == "__main__":
    print("=== Buffer Overflow Pattern Generator & Offset Finder ===")
    
    # 패턴 생성
    try:
        length = int(input("Enter the length of the pattern to generate: "))
        pattern = create_pattern(length)
        print(f"Generated pattern ({length} bytes):")
        print(pattern)
    except ValueError:
        print("Invalid input! Please enter a numeric value for length.")
        exit(1)
    
    # 특정 값 찾기
    search_value = input("\nEnter the value to find (e.g., 'aa1aa2'): ")
    result = find_offset(search_value, length)
    print(result)