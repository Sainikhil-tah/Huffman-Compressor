import flask
from flask import Flask,render_template,request,send_file,redirect
import subprocess,sys,os
from pathlib import Path
app=Flask(__name__)
UPLOAD='uploads'; OUTPUT='outputs'
os.makedirs(UPLOAD,exist_ok=True); os.makedirs(OUTPUT,exist_ok=True)

@app.route('/',methods=['GET','POST'])
def home():
    if request.method=='POST':
        f=request.files['file']; mode=request.form['mode']
        inp=os.path.join(UPLOAD,f.filename); f.save(inp)
        ext='.huff' if mode=='compress' else '_decoded.txt'
        out=os.path.join(OUTPUT,Path(f.filename).stem+ext)
        exe='huffman.exe' if os.name=='nt' else './huffman'
        subprocess.run([exe,mode,inp,out],check=True)
        return send_file(out,as_attachment=True)
    return render_template('index.html')

if __name__=='__main__':
    if len(sys.argv)>2:
        mode=sys.argv[1]; inp=sys.argv[2]
        out=str(Path(inp).with_suffix('.huff' if mode=='compress' else '.txt'))
        exe='huffman.exe' if os.name=='nt' else './huffman'
        subprocess.run([exe,mode,inp,out],check=True)
        print('Output:',out)
    else:
        app.run(debug=True)
