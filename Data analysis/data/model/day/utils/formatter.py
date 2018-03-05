import datetime

from data.model.day.day import Day


def read_day_format(text: str) -> Day:
    date = datetime.datetime.strptime(text, '%Y-%m-%d')

    return Day(
        date.day,
        date.month,
        date.year,
        date.weekday()
    )
