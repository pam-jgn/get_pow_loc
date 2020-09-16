import os, sys, time

import tkinter 

#files = ["C:/Users/buffa/source/repos/test_mysql/test_mysql/evalution.txt"]
files = ["evalution.txt"]


def pushed():
    frm.destroy()


def read_Files(Files):
  data = []
  for file in Files:
    with open(file) as f:
      s = f.read()
      # 分割
      lines=s.split('\n')
      data.extend(lines)
  return data


# ---  main --#
if __name__ == "__main__": 
    # Tkクラス生成
    frm = tkinter.Tk()
    # 画面サイズ
    frm.geometry('800x600')
    # 画面タイトル
    frm.title('消費電力状況')

    datas = read_Files(files)

    label = tkinter.Label(frm, text=datas[0], font=(u'ＭＳ ゴシック', 16))
    label.pack()
    label2 = tkinter.Label(frm, text=datas[1], font=(u'ＭＳ ゴシック', 16))
    label2.pack()

    button = tkinter.Button(frm, text="OK", height = 2, width = 20, bg='#fff' , foreground = 'red', font=(u'ＭＳ ゴシック', 20), command=pushed)
    button.pack()


    # 画面をそのまま表示
    frm.mainloop()