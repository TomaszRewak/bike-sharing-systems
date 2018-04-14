from typing import List


def load_days(
        file_path: str
) -> List[str]:
    with open(file_path, mode='r') as file:
        header_line = file.readline()
        days_number = int(header_line)

        result = []
        for _ in range(days_number):
            day_line = file.readline().replace('\n', '')

            result.append(day_line)

        return result
