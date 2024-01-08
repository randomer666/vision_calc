import asyncio
from playwright.async_api import async_playwright
import csv
from bs4 import BeautifulSoup
import random
import re

# List of user agents to rotate
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:86.0) Gecko/20100101 Firefox/86.0",
    # Add more user-agents as needed
]

async def scrape_site():
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=True)
        context = await browser.new_context(user_agent=random.choice(user_agents))
        page = await context.new_page()

        target_url = 'https://www.bhphotovideo.com/c/search?q=kodak%20film%2016mm&filters=fct_category%3Amovie_film_341%2Cfct_format_246%3A16mm'
        response = await page.goto(target_url, wait_until='networkidle')

        if response.status == 403:
            print("Access Forbidden (403).")
            await browser.close()
            return None
        elif response.status >= 400:
            print(f"Error: Page returned status {response.status}.")
            await browser.close()
            return None

        content = await page.content()
        await browser.close()
        return content

def parse_product_title(title):
    pattern = r'(\w+) (\w+3) (\d+T) Color Negative Film (#\d+) \((\d+mm), (\d+\' Roll), (.+?)\)'
    match = re.match(pattern, title)

    if match:
        return {
            "Brand": match.group(1),
            "Type": match.group(2),
            "ISO": match.group(3),
            "Number": match.group(4),
            "FORMAT": match.group(5),
            "Roll Length": match.group(6),
            "Perforation Type": match.group(7)
        }
    else:
        return {"Error": "Pattern not matched"}

def parse_and_save(content):
    soup = BeautifulSoup(content, 'html.parser')
    filename = "vision3_16.csv"

    # Find product names
    product_names = []
    for product in soup.find_all('div', attrs={'data-selenium': 'miniProductPageDescription'}):
        name_tag = product.find('h3', attrs={'data-selenium': 'miniProductPageName'})
        name = name_tag.text.strip() if name_tag else "No name found"
        product_names.append(name)

    # Find product prices
    product_prices = []
    for product in soup.find_all('div', attrs={'data-selenium': 'miniProductPagePricingContainer'}):
        price_tag = product.find('span', attrs={'data-selenium': 'uppedDecimalPriceFirst'})
        price = price_tag.text.strip() if price_tag else "No price found"
        product_prices.append(price)

    # Open a CSV file for writing
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerow(['Brand', 'Type', 'ISO', 'Number', 'FORMAT', 'Roll Length', 'Perforation Type', 'Price'])

        for name, price in zip(product_names, product_prices):
            parsed_data = parse_product_title(name)
            if "Error" not in parsed_data:
                writer.writerow([
                    parsed_data.get("Brand", ""),
                    parsed_data.get("Type", ""),
                    parsed_data.get("ISO", ""),
                    parsed_data.get("Number", ""),
                    parsed_data.get("FORMAT", ""),
                    parsed_data.get("Roll Length", ""),
                    parsed_data.get("Perforation Type", ""),
                    price
                ])
            else:
                print(f"Failed to parse product title: {name}")

    print(f"Data exported to {filename}")

async def main():
    html_content = await scrape_site()
    if html_content:
        parse_and_save(html_content)
    else:
        print("Failed to retrieve page content.")

asyncio.run(main())
