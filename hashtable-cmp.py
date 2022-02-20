# -*- coding: UTF-8 -*-
# python 3

import sys

g_Amount = 0

def analyse_hashtable(df, outfile_prefix='', mode='w', write_hti=False):
    if outfile_prefix == '':
        return

    # count the hash
    print('grouping the values ...')
    dfg = df.groupby(by=['Slots', 'Entries', 'Type'])

    print('statistic: counting the values ...')
    count = dfg['Value'].value_counts()

    # get the new DataFrame
    print('gen new DataFrame ...')
    count.name = 'Count'
    df = count.reset_index('Value')

    #
    df['Loops'] = (df['Count'] + 1) * df['Count'] / 2

    # big = slow
    if write_hti:
        print('writing hashtable indexes to file ...')
        df.to_csv(outfile_prefix +'.hti.csv', mode=mode, header=mode=='w')

    #
    print('gen statistics ...')
    #
    df = df.reset_index()
    df['isBad'] = df['Count'] > df['Entries']
    #
    dfg = df.groupby(by=['Slots', 'Entries', 'Type'])
    dfr = dfg['Loops'].describe()
    dfr['BadEntriesCount'] = dfg['isBad'].sum()
    dfr['Loops'] = dfg['Loops'].sum()
    dfr['Amount'] = g_Amount
    print('writing statistics ...')
    dfr.to_csv(outfile_prefix +'.statistics.csv', mode=mode, header=mode=='w')


def main():
    # parse argv
    argvs = sys.argv
    argc = len(argvs)
    if argc < 2:
        print('usage: %s <hashes-file> [write-intermediate-data]', argvs[0])
        return

    # a bit slow ~
    import pandas as pd

    print('reading file ...')
    df = pd.read_csv(filepath_or_buffer = argvs[1], sep = '\t')

    #
    print('melting DataFrame to 1 column ...')
    df = df.melt(id_vars=['string'], var_name='Type', value_name='Value')

    #
    global g_Amount
    g_Amount = df['string'].count()
    df['Slots'] = 0xFFFFFFFF
    df['Entries'] = 1

    #
    outfile_prefix = argvs[1]
    print('')

    #
    analyse_hashtable(df, outfile_prefix, 'w', argc > 2)

    #
    print('indexing for hashtabl[?]:')
    sizes = [1000003, 100003, 10007, 28672, 57344, 28669, 57347]
    for size in sizes:
        print('')
        df2 = df.copy()
        print('%d ...' % size)
        df2['Value'] = df2['Value'].transform(lambda x: int(x, 16)) % size
        df2['Slots'] = size
        df2['Entries'] = g_Amount // size + (g_Amount % size > 0)
        #
        analyse_hashtable(df2, outfile_prefix, 'a', argc > 2)


if __name__ == '__main__':
    main()
