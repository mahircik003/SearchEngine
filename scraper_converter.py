import asyncio
import httpx
from bs4 import BeautifulSoup
import requests
from urllib.parse import urljoin, urlparse

import re



visited_websites = set()

def normalize_url(url):
    # Adding trailing slash and ensure a consistent format
    normalized_url = urljoin(url, urlparse(url).path)
    if not normalized_url.endswith('/'):
        normalized_url += '/'
    return normalized_url

def scrape_links_and_words(url, output_file, max_depth=3, current_depth=0):
    global visited_websites

    try:
        normalized_url = normalize_url(url)
        # Checkong if the website has already been visited
        if normalized_url in visited_websites:
            return

        # Sending GET request 
        response = requests.get(normalized_url)

        if response.status_code == 200:#successful
            soup = BeautifulSoup(response.text, 'html.parser')

            # Extracting...
            if normalized_url not in visited_websites:
                with open(output_file, 'a', encoding='utf-8') as file:
                    file.write(f"{normalized_url}\n")

            # Extracting...
            text_content = soup.get_text()

            # Writing each word on a new line in the output file
            with open(output_file, 'a', encoding='utf-8') as file:
                for word in text_content.split():
                    file.write(word + '\n')

            # Adding \n character to separate data from different websites
            with open(output_file, 'a', encoding='utf-8') as file:
                file.write('\n')

            # Adding the current website to the set of visited websites
            visited_websites.add(normalized_url)

            # Recursive definition
            if current_depth < max_depth:
                links = soup.find_all('a')
                for link in links:
                    href = link.get('href')
                    if href:
                        # Convert relative URLs to absolute URLs
                        absolute_url = urljoin(normalized_url, href)

                        # Filter out non-HTTP(S) links
                        if absolute_url.startswith(('http://', 'https://')):
                            scrape_links_and_words(absolute_url, output_file, max_depth, current_depth + 1)

        else:
            print(f"Failed to retrieve the page. Status code: {response.status_code}")

    except UnicodeEncodeError as e:
        print(f"UnicodeEncodeError occurred: {e}. Skipping...")
    except Exception as e:
        print(f"An error occurred: {e}. Terminating...")


def remove_non_alphanumeric_and_lowercase_except_websites(input_file, output_file):
    try:
        with open(input_file, 'r', encoding='utf-8') as file:
            content = file.read()

            lines = content.split('\n')
            cleaned_lines = []

            for line in lines:
                # Checking if the line contains "https://" or "http://"
                if "https://" in line or "http://" in line:
                    cleaned_lines.append(line)
                else:
                    # Remove non-alphanumeric characters
                    cleaned_line = re.sub(r'[^a-zA-Z0-9\s]', '', line)
                    cleaned_line = cleaned_line.lower()
                    if cleaned_line:  # Checking if the line is not empty after cleaning
                        cleaned_lines.append(cleaned_line)

            cleaned_content = '\n'.join(cleaned_lines)

        with open(output_file, 'w', encoding='utf-8') as file:
            file.write(cleaned_content)

    except Exception as e:
        print(f"An error occurred: {e}")


async def extract_links(session, url):
    try:
        response = await session.get(url)
        soup = BeautifulSoup(response.text, 'lxml')
        links = [a['href'] for a in soup.find_all('a', href=True)]
        return links
    except Exception as e:
        return []

async def create_accessibility_matrix(urls):
    matrix = [[0] * len(urls) for _ in range(len(urls))]

    async with httpx.AsyncClient() as session:
        tasks = [extract_links(session, url) for url in urls]
        results = await asyncio.gather(*tasks)

        for i, links_from_url_i in enumerate(results):
            for j, url_j in enumerate(urls):
                matrix[i][j] = 1 if url_j in links_from_url_i else 0

    return matrix




output_file = 'output.txt'
initial_url = 'https://ceng.metu.edu.tr/undergraduate-curriculum'
scrape_links_and_words(initial_url, output_file, 1)


remove_non_alphanumeric_and_lowercase_except_websites('output.txt', 'output.txt')



urls = []


with open('output.txt', 'r') as output_file:
    text_content=output_file.read()


for word in text_content.split():
    if word.startswith(('http://', 'https://')):
        urls.append(word)



loop = asyncio.get_event_loop()
result_matrix = loop.run_until_complete(create_accessibility_matrix(urls))



filename = "matrix.txt"


with open(filename, 'w') as file:
    for row in result_matrix:
        row_str = " ".join(map(str, row))
        file.write(f"{row_str}\n")
