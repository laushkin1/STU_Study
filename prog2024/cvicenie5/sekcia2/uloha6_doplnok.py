from datetime import datetime
from decimal import Decimal
from dateutil.relativedelta import relativedelta

def hanojske_veze_64() -> str:
    days = Decimal((2**64) - 1)
    now = datetime.now()
    years_to_add = int(days/Decimal(365.25))
    remaining_days = int(days % Decimal(365.25))

    future_date = now + relativedelta(days=remaining_days)
    years_away = years_to_add + (remaining_days / 365.25)
    return f"To end of the world is {years_away} years away\nIt will happen on {int(future_date.strftime("%Y")) + years_to_add}-{future_date.strftime("%m-%d")}"

if __name__ == "__main__":
    print(hanojske_veze_64())

