import pandas as pd

class DataSaver:
    def __init__(self):
        self.data = []

    def update(self, new_point):
        self.data.append(new_point) # TODO: make this better

    def save(self, filename):
        # conver data to a pandas dataframe
        data_df = pd.DataFrame(self.data)
        data_df.to_csv(filename, index=False)


if __name__ == "__main__":
    ds = DataSaver()
    for i in range(10):
        for j in range(10):
            for k in range(10):
                new_point = (i,j,k)
                ds.update(new_point)
    # data_df = pd.DataFrame(ds.data)
    # print(data_df)
    ds.save('test.csv')
