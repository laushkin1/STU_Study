from datetime import timedelta

sekundy = 999
timedt_str = str(timedelta(seconds=sekundy)).split(':')

print(f"Time in sec: {sekundy}\n")
print(timedt_str[0], 'Hours')
print(timedt_str[1], 'Minutes')
print(timedt_str[2], 'Seconds')

