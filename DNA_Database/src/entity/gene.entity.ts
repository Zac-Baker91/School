import { BaseEntity, Column, ColumnTypeUndefinedError, Entity, JoinColumn, JoinTable, ManyToMany, OneToOne, ManyToOne,OneToMany, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";

import {Cell} from "./cell.entity"
import { Locus } from "./locus.entity";
@Entity()
export class MotifiInst {
    @PrimaryColumn()
    name: string

    @Column()
    end: number

    @Column()
    chr_num: number

    @Column()
    pos: number

    @Column()
    Expression: number

    @ManyToMany(()=>Locus)
    locus_id: Locus

    @ManyToOne(()=>Cell, (cell)=>cell.type)
    cellType: Cell


}