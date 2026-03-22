// person.k - Test module with class export
export class Person {
    fn init(name, age) {
        this.name = name
        this.age = age
    }
    
    fn greet() {
        return "Hello, I'm " + this.name + " and I'm " + this.age + " years old"
    }
}

export fn createPerson(name, age) {
    return new Person(name, age)
}
