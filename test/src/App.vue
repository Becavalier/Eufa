<template>
  <div id="app">
    <h1>Eufa</h1>
    <h4>A low-level high efficient utility functions library written in webassembly.</h4>
    <br/>
    <div>
      <h3>Math</h3>
      <span>Eufa.Math.i64_add({{ params.i_x }}, {{ params.i_y }}): <b>{{ i64_add }}</b></span><br/>
      <span>Eufa.Math.f64_add({{ params.f_x }}, {{ params.f_y }}): <b>{{ f64_add }}</b></span><br/>
      <span>Eufa.Math.i64_minus({{ params.i_x }}, {{ params.i_y }}): <b>{{ i64_minus }}</b></span><br/>
      <span>Eufa.Math.f64_minus({{ params.f_x }}, {{ params.f_y }}): <b>{{ f64_minus }}</b></span><br/>
      <span>Eufa.Math.i64_multiply({{ params.i_x }}, {{ params.i_y }}): <b>{{ i64_multiply }}</b></span><br/>
      <span>Eufa.Math.f64_multiply({{ params.f_x }}, {{ params.f_y }}): <b>{{ f64_multiply }}</b></span><br/>
      <span>Eufa.Math.i64_divide({{ params.i_x }}, {{ params.i_y }}): <b>{{ i64_divide }}</b></span><br/>
      <span>Eufa.Math.f64_divide({{ params.f_x }}, {{ params.f_y }}): <b>{{ f64_divide }}</b></span><br/>
      <span>Eufa.Math.i64_abs({{ params.ni_x }}): <b>{{ i64_abs }}</b></span><br/>
      <span>Eufa.Math.f64_abs({{ params.nf_x }}): <b>{{ f64_abs }}</b></span><br/>
      <span>Eufa.Math.i64_sqrt({{ params.i_x }}): <b>{{ i64_sqrt }}</b></span><br/>
      <span>Eufa.Math.f64_sqrt({{ params.f_x }}): <b>{{ f64_sqrt }}</b></span><br/>

      <h3>String</h3>
      <span>Eufa.String.ptr_capitalize('{{ params.string_en }}'): <b>{{ ptr_capitalize }}</b></span><br/>

      <h3>Encryptor</h3>
      <span>Eufa.Encryptor.base64_encode('{{ params.string_zh }}'): <b>{{ base64_encode_zh }}</b></span><br/>
      <span>Eufa.Encryptor.base64_encode('{{ params.string_en }}'): <b>{{ base64_encode_en }}</b></span><br/>
      <span>Eufa.Encryptor.base64_decode('{{ params.string_zh_base64_encoded }}'): <b>{{ base64_decode_zh }}</b></span><br/>
      <span>Eufa.Encryptor.base64_decode('{{ params.string_en_base64_encoded }}'): <b>{{ base64_decode_en }}</b></span><br/>
      <span>Eufa.Encryptor.md5('{{ params.string_en }}'): <b>{{ md5_en }}</b></span><br/>
      <span>Eufa.Encryptor.md5('{{ params.string_zh }}'): <b>{{ md5_zh }}</b></span><br/>
      <span>Eufa.Encryptor.sha1('{{ params.string_en }}'): <b>{{ sha1_en }}</b></span><br/>
      <span>Eufa.Encryptor.sha1('{{ params.string_zh }}'): <b>{{ sha1_zh }}</b></span><br/>
    </div>
  </div>
</template>

<script>
import eufa from 'eufa'

export default {
  name: 'app',
  data () {
    return {
      eufa: null,
      params: {
        i_x: 10,
        i_y: 20,
        f_x: 11.5,
        f_y: 22.5,
        ni_x: -10,
        ni_y: -20,
        nf_x: -11.5,
        nf_y: -22.5,
        string_en: 'abcdefg',
        string_zh: '你好 WebAssembly',
        string_en_base64_encoded: 'YWJjZGVmZwA=',
        string_zh_base64_encoded: '5L2g5aW9IFdlYkFzc2VtYmx5AA=='
      },
      i64_add: '',
      f64_add: '',
      i64_minus: '',
      f64_minus: '',
      i64_multiply: '',
      f64_multiply: '',
      i64_divide: '',
      f64_divide: '',
      i64_abs: '',
      f64_abs: '',
      i64_sqrt: '',
      f64_sqrt: '',
      ptr_capitalize: '',
      base64_encode_zh: '',
      base64_encode_en: '',
      base64_decode_zh: '',
      base64_decode_en: '',
      md5_en: '',
      md5_zh: '',
      sha1_en: '',
      sha1_zh: ''
    }
  },
  created () {
    eufa.init('static/eufa-module.wasm', () => {
      console.log(eufa)
      // Call directly
      // Math.add
      this.i64_add = eufa.Math.i64_add(this.params.i_x, this.params.i_y)
      this.f64_add = eufa.Math.f64_add(this.params.f_x, this.params.f_y)
      // Math.minus
      this.i64_minus = eufa.Math.i64_minus(this.params.i_x, this.params.i_y)
      this.f64_minus = eufa.Math.f64_minus(this.params.f_x, this.params.f_y)
      // Math.multiply
      this.i64_multiply = eufa.Math.i64_multiply(this.params.i_x, this.params.i_y)
      this.f64_multiply = eufa.Math.f64_multiply(this.params.f_x, this.params.f_y)
      // Math.divide
      this.i64_divide = eufa.Math.i64_divide(this.params.i_y, this.params.i_x)
      this.f64_divide = eufa.Math.f64_divide(this.params.f_x, this.params.f_y)
      // Math.abs
      this.i64_abs = eufa.Math.i64_abs(this.params.ni_x)
      this.f64_abs = eufa.Math.f64_abs(this.params.nf_x)
      // Math.sqrt
      this.i64_sqrt = eufa.Math.i64_sqrt(this.params.i_x)
      this.f64_sqrt = eufa.Math.f64_sqrt(this.params.f_x)
      // String.capitalize
      this.ptr_capitalize = eufa.String.capitalize(this.params.string_en)
      // Encryptor.base64_encode
      this.base64_encode_zh = eufa.Encryptor.base64_encode(this.params.string_zh)
      this.base64_encode_en = eufa.Encryptor.base64_encode(this.params.string_en)
      this.base64_decode_zh = eufa.Encryptor.base64_decode(this.params.string_zh_base64_encoded)
      this.base64_decode_en = eufa.Encryptor.base64_decode(this.params.string_en_base64_encoded)
      // Encryptor.md5
      this.md5_en = eufa.Encryptor.md5(this.params.string_en)
      this.md5_zh = eufa.Encryptor.md5(this.params.string_zh)
      // Encryptor.sha1
      this.sha1_en = eufa.Encryptor.sha1(this.params.string_en)
      this.sha1_zh = eufa.Encryptor.sha1(this.params.string_zh)
    })
  }
}

</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
