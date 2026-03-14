import random


BUFFER_SIZE = 8


transaction_pages = [random.randint(1, 15) for _ in range(50)]



def fifo_page_replacement(pages, capacity):

    memory = []
    page_faults = 0
    index = 0

    for page in pages:

        if page not in memory:
            page_faults += 1

            if len(memory) < capacity:
                memory.append(page)

            else:
                memory[index] = page
                index = (index + 1) % capacity

    return page_faults



def optimal_page_replacement(pages, capacity):

    memory = []
    page_faults = 0

    for i in range(len(pages)):
        page = pages[i]

        if page not in memory:

            page_faults += 1

            if len(memory) < capacity:
                memory.append(page)

            else:
                future = pages[i+1:]
                index_to_replace = -1
                farthest = -1

                for j in range(len(memory)):

                    if memory[j] not in future:
                        index_to_replace = j
                        break
                    else:
                        next_use = future.index(memory[j])

                        if next_use > farthest:
                            farthest = next_use
                            index_to_replace = j

                memory[index_to_replace] = page

    return page_faults


print("Transaction Access Sequence:")
print(transaction_pages)

fifo_faults = fifo_page_replacement(transaction_pages, BUFFER_SIZE)
optimal_faults = optimal_page_replacement(transaction_pages, BUFFER_SIZE)

print("\nBuffer Size:", BUFFER_SIZE)
print("Total Transactions:", len(transaction_pages))

print("\nFIFO Page Faults:", fifo_faults)
print("Optimal Page Faults:", optimal_faults)

fifo_rate = fifo_faults / len(transaction_pages)
optimal_rate = optimal_faults / len(transaction_pages)

print("\nFIFO Page Fault Rate:", round(fifo_rate, 3))
print("Optimal Page Fault Rate:", round(optimal_rate, 3))