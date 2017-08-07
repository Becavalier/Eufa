<template>
  <div id="app">
    <h1>Eufa</h1>
    <h4>A low-level high efficient utility functions library written in webassembly.</h4>
    <br/>
    <div>
      <h3>Math</h3>
      <span>Eufa.Math.i64_add({{ params.i_x }}, {{ params.i_y }}): {{ i64_add }}</span><br/>
      <span>Eufa.Math.f64_add({{ params.f_x }}, {{ params.f_y }}): {{ f64_add }}</span><br/>
      <span>Eufa.Math.i64_minus({{ params.i_x }}, {{ params.i_y }}): {{ i64_minus }}</span><br/>
      <span>Eufa.Math.f64_minus({{ params.f_x }}, {{ params.f_y }}): {{ f64_minus }}</span><br/>
      <span>Eufa.Math.i64_multiply({{ params.i_x }}, {{ params.i_y }}): {{ i64_multiply }}</span><br/>
      <span>Eufa.Math.f64_multiply({{ params.f_x }}, {{ params.f_y }}): {{ f64_multiply }}</span><br/>
      <span>Eufa.Math.i64_divide({{ params.i_x }}, {{ params.i_y }}): {{ i64_divide }}</span><br/>
      <span>Eufa.Math.f64_divide({{ params.f_x }}, {{ params.f_y }}): {{ f64_divide }}</span><br/>
      <span>Eufa.Math.i64_abs({{ params.ni_x }}): {{ i64_abs }}</span><br/>
      <span>Eufa.Math.f64_abs({{ params.nf_x }}): {{ f64_abs }}</span><br/>
      <span>Eufa.Math.i64_sqrt({{ params.i_x }}): {{ i64_sqrt }}</span><br/>
      <span>Eufa.Math.f64_sqrt({{ params.f_x }}): {{ f64_sqrt }}</span><br/>
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
        nf_y: -22.5
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
      f64_sqrt: ''
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
