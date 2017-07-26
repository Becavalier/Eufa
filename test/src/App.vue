<template>
  <div id="app">
    <h3>Math</h3>
    <span>Eufa.Math.i64_add({{ i64_add.params.x }}, {{ i64_add.params.y }}): {{ i64_add.result }}</span><br/>
    <span>Eufa.Math.f64_add({{ f64_add.params.x }}, {{ f64_add.params.y }}): {{ f64_add.result }}</span><br/>
    <span>Eufa.Math.i64_minus({{ i64_minus.params.x }}, {{ i64_minus.params.y }}): {{ i64_minus.result }}</span><br/>
    <span>Eufa.Math.f64_minus({{ f64_minus.params.x }}, {{ f64_minus.params.y }}): {{ f64_minus.result }}</span><br/>
    <span>Eufa.Math.i64_multiply({{ i64_multiply.params.x }}, {{ i64_multiply.params.y }}): {{ i64_multiply.result }}</span><br/>
    <span>Eufa.Math.f64_multiply({{ f64_multiply.params.x }}, {{ f64_multiply.params.y }}): {{ f64_multiply.result }}</span><br/>
    <span>Eufa.Math.i64_divide({{ i64_divide.params.y }}, {{ i64_divide.params.x }}): {{ i64_divide.result }}</span><br/>
    <span>Eufa.Math.f64_divide({{ f64_divide.params.x }}, {{ f64_divide.params.y }}): {{ f64_divide.result }}</span>
  </div>
</template>

<script>
export default {
  name: 'app',
  data () {
    return {
      eufa: null,
      i64_add: {
        params: {
          x: 10,
          y: 20
        },
        result: ''
      },
      f64_add: {
        params: {
          x: 10.1,
          y: 20.2
        },
        result: ''
      },
      i64_minus: {
        params: {
          x: 10,
          y: 20
        },
        result: ''
      },
      f64_minus: {
        params: {
          x: 10.1,
          y: 20.2
        },
        result: ''
      },
      i64_multiply: {
        params: {
          x: 10,
          y: 20
        },
        result: ''
      },
      f64_multiply: {
        params: {
          x: 10.1,
          y: 20.2
        },
        result: ''
      },
      i64_divide: {
        params: {
          x: 10,
          y: 20
        },
        result: ''
      },
      f64_divide: {
        params: {
          x: 10.1,
          y: 20.2
        },
        result: ''
      }
    }
  },
  created () {
    window.Eufa.init.install('static/eufa.wasm', 'static/eufa.js', () => {
      // Call directly
      // Math.add
      this.i64_add.result = window.Eufa.Math.i64_add(this.i64_add.params.x, this.i64_add.params.y)
      this.f64_add.result = window.Eufa.Math.f64_add(this.f64_add.params.x, this.f64_add.params.y)
      // Math.minus
      this.i64_minus.result = window.Eufa.Math.i64_minus(this.i64_minus.params.x, this.i64_minus.params.y)
      this.f64_minus.result = window.Eufa.Math.f64_minus(this.f64_minus.params.x, this.f64_minus.params.y)
      // Math.multiply
      this.i64_multiply.result = window.Eufa.Math.i64_multiply(this.i64_multiply.params.x, this.i64_multiply.params.y)
      this.f64_multiply.result = window.Eufa.Math.f64_multiply(this.f64_multiply.params.x, this.f64_multiply.params.y)
      // Math.divide
      this.i64_divide.result = window.Eufa.Math.i64_divide(this.i64_divide.params.y, this.i64_divide.params.x)
      this.f64_divide.result = window.Eufa.Math.f64_divide(this.f64_divide.params.x, this.f64_divide.params.y)
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
