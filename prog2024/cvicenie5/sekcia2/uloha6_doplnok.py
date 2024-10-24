from datetime import datetime, timedelta

def hanojske_veze_64() -> str:
    days = (2**64) - 1
    now = datetime.now()
    future_date = now + timedelta(days=days)
    years_away = (future_date - now).days / 365.25
    return f"To end of the world is {years_away} years away\nIt will happen on {str(future_date)}"

if __name__ == "__main__":
    print(hanojske_veze_64())

