const sqlite3 = require('sqlite3').verbose();
const faker = require('faker');

// open the database
let db = new sqlite3.Database('../test.db', sqlite3.OPEN_READWRITE, (err) => {
  if (err) {
    console.error(err.message);
  }
  console.log('Connected to the test.db database.');
});

// db.serialize(() => {
//   db.each(`SELECT MSSV, NAME, BIRTH,ADDRESS
//            FROM students`, (err, row) => {
//     if (err) {
//       console.error(err.message);
//     }
//   });
// });

function genrateData() {
	var randomMSSV = faker.random.number(99999);
	randomMSSV += 20100000;

	var randomName = faker.name.findName();

	var randomBirth = faker.date.past().toLocaleDateString("en-US");

	var randomAddress = faker.address.streetAddress();
	randomAddress += ", " +faker.address.city();
	randomAddress += ", " +faker.address.country();

	return {randomMSSV, randomName, randomBirth, randomAddress};
}


db.serialize(async function() {
  // db.run("CREATE TABLE students2 (MSSV TEXT, NAME TEXT, BIRTH TEXT, ADDRESS TEXT)");
  var stmt = await db.prepare('INSERT INTO students VALUES (?, ?, ?, ?)');
  for (var i = 0; i < 10000; i++) {
  	var {randomMSSV, randomName, randomBirth, randomAddress} = await genrateData(); 
  	console.log (`${randomMSSV} ${randomName}, ${randomBirth}, ${randomAddress}`)
    await stmt.run(`${randomMSSV}`, `${randomName}`, `${randomBirth}`, `${randomAddress}`);
    // await stmt.run(` NULL, NULL, NULL, NULL `);
  }
  await stmt.finalize();

  // db.each("SELECT rowid AS MSSV, NAME FROM students", function(err, row) {
  //     console.log(row.id + ": " + row.info);
  // });
});

db.close((err) => {
  if (err) {
    console.error(err.message);
  }
  console.log('Close the database connection.');
});

